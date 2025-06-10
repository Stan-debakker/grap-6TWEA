from time import time
tijd=time()
frequentie_map:dict[float,tuple[int,int]]=dict()
base_frequentie_list:list[float]=[334.53125,375,250,172.265625,187.5,125,86.328125,93.75]
for multiply in range(32):
    for base_frequentie_index in range(8):
        frequentie_map[multiply*base_frequentie_list[base_frequentie_index]]=base_frequentie_index,multiply
def find_frequentie(target:float)->tuple[float,float,tuple[int,int]]:
    """
    :exception ValueError: kan geen negatieve targets axepteren
    :param target: welke frequentie wil je produceren met de muziekkaart
    :return: dichtsbijzijnde frequentie bij target dat de muziekkaart kan produceren
             het verschil tussen de dichtsbijzijnde frequentie en het target
             (base_frequency_index,multiply) dat je moet ingeven in de muziekkaart.
    """
    if target<0:
        raise ValueError('negatieve frequencies zijn onmogelijk')
    delta_frequentie:float=target
    best_frequentie:float=0
    for frequentie in frequentie_map.keys():
        if delta_frequentie>abs(frequentie-target):
            delta_frequentie=abs(frequentie-target)
            best_frequentie=frequentie
    return best_frequentie,delta_frequentie,frequentie_map[best_frequentie]
print(find_frequentie(float(input("welke frequentie wil je gebruiken "))))