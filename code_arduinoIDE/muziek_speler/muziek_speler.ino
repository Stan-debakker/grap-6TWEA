#define settings 0
#define playback_commands 1
#define file_commands 2

#define EOL_CR 0
#define EOL_LF 1
#define EOL_CRLF 2

#define mode_read 0
#define mode_write 1
#define mode_read_write_end 2
#define mode_read_write_begin 3

#define promt_char '\r'

//ont comentaar als arduino mega
//#define Serial_muziek Serial1
#ifndef Serial_muziek
#define Serial_muziek Serial0
#endif

#define Dev_mode

const uint32_t temp[]={9600,19200,38400,57600,115200,2400,4800,230400,460800};
//structs
  struct play_back_info {
    uint8_t error=0;  //enkel as er een error is gebeurt
    uint16_t position;
    uint8_t samplerate;
    uint16_t bitrate;
    char channels;
  };
  struct play_back_status{
    uint8_t error=0;  //enkel as er een error is gebeurt
    uint16_t tijd;
    uint8_t aantal_keer;
    char status;  //P: speelt,S: gestopt,D: gepouzeert
  };
  struct versie {
    uint8_t main_versie;
    uint8_t sub_versie;
    bool beta;
    uint8_t beta_vesie;
    char model[5];
  };
  struct free_handeler{
    uint8_t error=0;
    uint8_t posietie=0;
  };
  struct file_info{
    uint8_t error=0;
    uint32_t huidige_positie=0;
    uint32_t grote=0;
  };
  struct char_return{
    uint8_t error=0;
    uint8_t data;
  };

void setup() {
  delay(500);
  Serial.begin(115200);
  for (uint8_t i=0;i<=8;i++){
    Serial.print(temp[i]);
    Serial_muziek.begin(temp[i]);
    Serial.print('\t');
    Serial.println(_set_bitrate(4),HEX);
    Serial_muziek.end();
    delay(1000);
  }
  uint32_t bit_rate=temp[3];
  Serial.println(bit_rate);
  Serial_muziek.begin(bit_rate);
  Serial_muziek.write('\r');
  Serial_muziek.write('V');
  Serial_muziek.write('\r');
}

void loop() {
  if (_available()){
    Serial.print(Serial_muziek.read());
  }
  if (Serial.available()>0){
    _write_char(Serial.read());
  }
}

void sincronize_transmission(){
  _write_char('\r');
  char c=0;
  while (_available()&&c=='>') {
    c=_read_char();
  };
}
//instellingen
  uint8_t set_EOL_as(uint8_t mode){
    char data[2]={0,0};
    switch (mode){
      case EOL_CR:
        data[0]='0';
        break;
      case EOL_LF:
        data[0]='1';
        break;
      case EOL_CRLF:
        data[0]='2';
        break;
      default:
        return 6; //command formating error
    }
    _send_command('E',data,settings);
    return _get_return_error();
  }
  #ifdef Dev_mode
  #warning "you are now in dev mode and you have now acses to functions that make it unuzeble"
  uint8_t set_write_timeout(uint8_t tijd){
    if (tijd!=255){
      char data[4];
      sprintf(data,"%dn",tijd);
      _send_command('T',data,settings);
      return _get_return_error();
    }else{
      return 6; //command formating error
    }
  }
  uint8_t set_response_delay(uint8_t tijd){
    if (tijd!=255){
      char data[4];
      sprintf(data,"%dn",tijd);
      _send_command('R',data,settings);
      return _get_return_error();
    }else{
      return 6; //command formating error
    }
  }
  uint8_t set_list_mode(bool grote){
    if (grote){
      _send_command('L',"0",settings);
    }else{
      _send_command('L',"1",settings);
    }
    return _get_return_error();
  }
  uint8_t _set_bitrate(uint8_t waarde){
    if (waarde<8){
      char data[2]={waarde+'0','\n'};
      _send_command('D',data,settings);
      //return _get_return_error();
    }else{
      return 6; //command formating error
    }
  }
  uint8_t _set_hardware_bussy(bool actief){  //kan niet worden gebruikt niet geconecteerd
    _send_command('H',(actief?"0":"1"),settings);
    return _get_return_error();
  }
  uint8_t _input_style(uint8_t mode){
    if (mode<3){
      char data[2]={mode+'0','\n'};
      _send_command('S',data,settings);
      return _get_return_error();
    }else{
      return 6; //command formating error
    }
  }
  uint8_t _set_prompt_char(char karakter){
    char data[4];
    sprintf(data,"%dn",karakter);
    _send_command('D',data,settings);
    return _get_return_error();
  }
  #endif
//speel commando's
  uint8_t play_file(char* path){
    _send_command('F',path,playback_commands);
    return _get_return_error();
  }
  uint8_t set_volume(uint8_t links,uint8_t rechts){
    if (links!=255 & rechts!=255){
      if (links==rechts){
        char data[4];
        sprintf(data,"%d\n",links);
        _send_command('V',data,playback_commands);
        return _get_return_error();
      }else{
        char commando[8];
        sprintf(commando,"%d %d\n",links,rechts);
        _send_command('V',commando,playback_commands);
        return _get_return_error();
      }
    }else{
      return 6; //command formating error
    }
  }
  uint8_t herhaal(uint8_t aantal_keer){ //0 is een oneindig aantal keer
    char data[4];
    sprintf(data,"%d\n",aantal_keer);
    _send_command('O',data,playback_commands);
    return _get_return_error();
  }
  uint8_t jump(uint16_t tijd){
    char data[6];
    sprintf(data,"%d\n",tijd);
    _send_command('J',data,playback_commands);
    return _get_return_error();
  }
  uint8_t speed(uint8_t persentage){
    if (persentage<=250&persentage>=90){
      char commando[4];
      sprintf(commando,"%d\n",persentage);
      _send_command('X',commando,playback_commands);
      return _get_return_error();
    }
    return 6; //command formating error
  }
  uint8_t play_tone(uint8_t base_frequency_index,uint8_t multiply){
    //tone_frequency=base_frequency_index*d
    //base_frequency_index=0: 334,53125
    //base_frequency_index=1: 375
    //base_frequency_index=2: 250
    //base_frequency_index=3: 172,265625
    //base_frequency_index=4: 187,5
    //base_frequency_index=5: 125
    //base_frequency_index=6: 86,328125
    //base_frequency_index=7: 93,75
    if (base_frequency_index<=8){
      if (multiply<=31){
        char data[4];
        sprintf(data,"%d\n",((base_frequency_index<<5)+multiply));
        _send_command('T',data,playback_commands);
        return _get_return_error();
      }
    }
    return 4;
  }
  uint8_t boost(int8_t treble_amplitude,uint8_t treble_frequency,int8_t bass_amplitude,uint8_t bass_frequency){
    if (treble_amplitude>=-8&&treble_amplitude<=7){
      if (treble_frequency<=15){
        if (bass_amplitude>=-8&&bass_amplitude<=7){
          if (bass_frequency<=15){
            uint16_t boost_o=(treble_amplitude&0xf)<<12|(treble_frequency&0xf)<<8|(bass_amplitude&0xf)<<4|(bass_frequency&0xf);
            char data[6];
            sprintf(data,"%d\n",boost);
            _send_command('B',data,playback_commands);
            return _get_return_error();
          }
        }
      }
    }
    return 6; //command formating error
  }
  uint8_t pouse(){
    _send_command('P',"\n",playback_commands);
    return _get_return_error();
  }
  uint8_t stop(){
    _send_command('S','\n',playback_commands);
    return _get_return_error();
  }
  uint8_t reset_audio(){
    _send_command('R','\n',playback_commands);
    return _get_return_error();
  }
  play_back_info get_play_back_info(){
    sincronize_transmission();  //sincronizeer de bus
    play_back_info info;
    _send_command('I','\n',playback_commands);
    while(!_available()){}
    if (_peek_char()=='E'){
      info.error=_get_return_error();
    }else{
      info.position=_get_nummer(DEC);
      info.samplerate=_get_nummer(DEC);
      info.bitrate=_get_nummer(DEC);
      info.channels=_get_nummer(DEC);
    }
    return info;
  }
  play_back_status get_play_back_status(){
    sincronize_transmission();  //sincronizeer de bus
    play_back_status info;
    _send_command('I','\n',playback_commands);
    while(!_available()){}
    if (_peek_char()=='E'){
      info.error=_get_return_error();
    }else{
      info.status=_read_char();
      info.tijd=_get_nummer(DEC);
      info.aantal_keer=_get_nummer(DEC);
    }
    return info;
  }
//file commando's
  uint8_t close_file(uint8_t bestand){
    if (bestand<=4){
      if (bestand==0){
        _send_command('C','\n',file_commands);
      }else{
        char data[2]={bestand+'0','\n'};
        _send_command('C',data,file_commands);
      }
      return _get_return_error();
    }
    return 6; //command formating error
  }
  uint8_t change_time_stamp(uint16_t jaar,uint8_t maand,uint8_t dag,uint8_t uur,uint8_t minuten,uint8_t seconden,char* path){
    if(jaar<2000){
      return 6; //command formating error
    }else if(maand==0|maand>12){
      return 6; //command formating error
    }else if(dag==0|dag>31){
      return 6; //command formating error
    }else if(minuten==0|minuten>60){
      return 6; //command formating error
    }else if(seconden==0|seconden>60){
      return 6; //command formating error
    }
    char data[20+sizeof(path)];
    sprintf(data,"%d %d %d %d %d %d %s\n",jaar,maand,dag,uur,minuten,seconden,path);
    _send_command('D',data,file_commands);
    return _get_return_error();
  }
  uint8_t erase_file(char* path){
    _send_command('E',path,file_commands);
    return _get_return_error();
  }
  free_handeler get_free_handeler(){
    free_handeler data;
    _send_command('F','\n',file_commands);
    if (_peek_char()=='E'){
      data.error=_get_return_error();
    }else{
      data.posietie=_get_nummer(HEX);
    }
    return data;
  }
  file_info bestands_info(){
    file_info data;
    _send_command('E',"\n",file_commands);
    if (_peek_char()=='E'){
      data.error=_get_return_error();
    }else{
      data.huidige_positie=_get_nummer(DEC);
      data.grote=_get_nummer(DEC);
    }

  }
  uint8_t jump(uint32_t addr){
    char data[11];
    sprintf(data,"%d\n",addr);
    _send_command('J',data,file_commands);
    return _get_return_error();
  }
  uint8_t jump_end(){
    _send_command('J',"E\n",file_commands);
    return _get_return_error();
  }
  uint8_t change_name(char* path,char* old_name,char* new_name){
    char data[4+2*sizeof(path)+sizeof(old_name)+sizeof(new_name)];
    sprintf(data,"%s/%s %s/%s");
    _send_command('N',data,file_commands);
    return _get_return_error();
  };
  /*uint8_t open(uint8_t posietie,uint8_t mode,char* path){
    if (posietie>4 || posietie==0){
      return 6; //command formating error
    }else if (mode==mode_read_write_begin){
      char data[6+sizeof(path)];
      sprintf(data,"%d RW %s",posietie,path);
      _send_command('O',data,file_commands);
    }else{
      char data[5+sizeof(path)];
      sprintf(data,'%d   %s',posietie,path);
      switch(mode){
        case mode_read:
          data[2]='R';
          break;
        case mode_write:
          data[2]='W';
          break;
        case mode_read_write_end:
          data[2]='A';
          break;
      }
      _send_command('O',data,file_commands);
    }
  }*/
  char_return read(uint8_t posietie){
    char_return data;
    if (posietie>4 || posietie==0){
      data.error= 6; //command formating error
    }else{
      char send[4];
      sprintf(send,'%d 1\n',posietie);
      _send_command('R',send,file_commands);
      data.data=_read_char();
      if(_peek_char()=='>'){
        _read_char();
      }else{
        data.error=_get_return_error();
      }
    }
    return data;
  }
//private functions
  bool _available(){
    return Serial_muziek.available();
  }
  char _peek_char(){
    while (!_available()){}
    return Serial_muziek.peek();
  }
  char _read_char(){
    while (!_available()){}
    return Serial_muziek.read();
  }
  void _write_char(char data){
    Serial_muziek.write(data);
  }
  void _send_command(char instelling,char* opperands,uint8_t comando){
    switch (comando){
      case settings:
        _write_char('S');
        _write_char('T');
        break;
      case playback_commands:
        _write_char('P');
        _write_char('C');
        break;
      case file_commands:
        _write_char('F');
        _write_char('C');
        break;
    }
    _write_char(instelling);
    while (*opperands!='\n'){
      _write_char(*opperands);
    }
    _write_char(promt_char);
  }
  uint8_t _get_return_error(){
    char c;
    while(true){
      c=_read_char();
      if (c==promt_char){
        return 0; //alles goed
      }else if (c=='E'){
        return _get_nummer(HEX);
      }else if (c==' '){
        while (_available()) _read_char();  //data (clear de buffer)
        return 0; //alles goed
      }
    }
  }
  int32_t _get_nummer(uint8_t base){
    bool negatief=false;
    char c;
    int32_t getal;
    c=_read_char();
    if (c=='-'){
      negatief=true;
      c=_read_char();
    }
    while (isHexadecimalDigit(c)){
      if(isDigit(c)){
        c-='0'; //maar er een getal van
      }else if (isLowerCase(c)){
        c-='a'; //maak er een getal van
      }else{
        c-='A'; //maak er een getal van
      }
      if (c>=base){
        break;
      }
      getal=getal*base+c;
      c=_read_char();
    }
    return (negatief? -getal:getal);
  }
