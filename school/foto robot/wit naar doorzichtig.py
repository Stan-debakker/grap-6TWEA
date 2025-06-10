from time import time
tijd=time()
from PIL import Image
image = Image.open("image1.png")

pixels=image.load()
foto_nieuw =Image.new("RGBA", image.size, (0,0,0,0))
pixels_nieuw = foto_nieuw.load()
for x in range(foto_nieuw.width):
    for y in  range(foto_nieuw.height):
        if pixels[x,y][0]>=250 and pixels[x,y][1]>=250 and pixels[x,y][2]>=250:
            pixels_nieuw[x,y]=(0,0,0,0)
        else:
            pixels_nieuw[x,y]=pixels[x,y]

foto_nieuw.save("logo_school.png")
print(time()-tijd)
foto_nieuw.show()