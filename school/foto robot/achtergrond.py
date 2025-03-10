from time import time
tijd=time()
from PIL import Image
foto_nieuw =Image.new("RGBA", (255, 10), (0,0,0,0))
pixels_nieuw = foto_nieuw.load()
for y in range(10):
    for x in  range(255):
        pixels_nieuw[x,y]=(255,128,128,x)

foto_nieuw.save("achtergrond.png")
print(time()-tijd)
foto_nieuw.show()