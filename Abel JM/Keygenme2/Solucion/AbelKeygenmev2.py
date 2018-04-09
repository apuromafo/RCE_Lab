import struct
import string

s_lookup = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'

name = raw_input('Ingrese el nombre: ')
serial = ''

name = name.upper() + 'PERUCRACKERS'



for i in range(0,len(name)):
    char_name = name[i]

    pos_char_lut = s_lookup.index(char_name)

    aux_1 = pos_char_lut + len(name)


    if aux_1 < len(s_lookup):
        if aux_1 == 0:
            aux_1 += len(s_lookup)
    else:
        aux_1 -= len(s_lookup)

    char_lup = s_lookup[aux_1]
    serial = serial + char_lup


print 'Serial: '+serial
raw_input()
