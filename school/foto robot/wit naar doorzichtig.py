from time import time
tijd=time()
from PIL import Image
image = Image.open("image1.jpeg")
pixels=image.load()
for x in range(500):
    for y in  range(499):
        if pixels[x,y]==(255,255,255):
            pixels[x,y]=(255,255,255,0)

image.save("robot.png")
print(time()-tijd)
image.show()