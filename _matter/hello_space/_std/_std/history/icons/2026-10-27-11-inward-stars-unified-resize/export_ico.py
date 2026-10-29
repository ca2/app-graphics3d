from pathlib import Path
from PIL import Image
import struct,sys
folder=Path(sys.argv[1]);sizes=(256,128,64,48,32,24,16);chunks=[]
for size in sizes:
 im=Image.open(folder/f'icon-{size}.png').convert('RGBA')
 pixels=im.tobytes('raw','BGRA',0,-1)
 stride=((size+31)//32)*4;mask=bytearray(stride*size);alpha=im.getchannel('A').tobytes()
 for y in range(size):
  for x in range(size):
   if alpha[(size-1-y)*size+x]==0:mask[y*stride+x//8]|=1<<(7-x%8)
 header=struct.pack('<IiiHHIIiiII',40,size,size*2,1,32,0,len(pixels)+len(mask),0,0,0,0)
 chunks.append(header+pixels+mask)
offset=6+16*len(sizes);directory=[]
for size,chunk in zip(sizes,chunks):
 directory.append(struct.pack('<BBBBHHII',size%256,size%256,0,0,1,32,len(chunk),offset));offset+=len(chunk)
(folder/'icon.ico').write_bytes(struct.pack('<HHH',0,1,len(sizes))+b''.join(directory)+b''.join(chunks))
