import os
from PIL import Image

directory = 'learning_data_base'

count = 3000
for filename in os.listdir(directory):
    if filename.endswith('.jpg'):
        count+=1
        f = os.path.join(directory,filename)
        img = Image.open(f)
        img.show()
        new = str(input())
        new += str(count)
        new += ".png"
        n = os.path.join(directory,new)
        os.rename(f,n)

