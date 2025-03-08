from time import time
tijd=time()
from PIL import Image
image = Image.open("image1.jpeg")
pixels=image.load()
foto_nieuw =Image.new("RGBA", (500, 499), (0,0,0,0))
pixels_nieuw = foto_nieuw.load()
for x in range(500):
    for y in  range(499):
        if pixels[x,y]==(255,255,255):
            pixels_nieuw[x,y]=(0,255,255,0)
        else:
            pixels_nieuw[x,y]=pixels[x,y]

foto_nieuw.save("robot.png")
print(time()-tijd)
foto_nieuw.show()