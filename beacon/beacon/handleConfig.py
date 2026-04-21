import sys, getopt
from Crypto.Cipher import AES
import os
from os import urandom
import hashlib
import random
import string
import subprocess
from pathlib import Path


characters = string.ascii_letters + string.digits
password = ''.join(random.choice(characters) for i in range(16))


KEY_XOR = password.replace('"','-').replace('\'','-')
KEY_AES = urandom(16)


def pad(s):
	return s + (AES.block_size - len(s) % AES.block_size) * chr(AES.block_size - len(s) % AES.block_size).encode('ISO-8859-1')


def aesenc(plaintext, key):
	k = hashlib.sha256(key).digest()
	iv = 16 * b'\x00'
	plaintext = pad(plaintext)    
	cipher = AES.new(k , AES.MODE_CBC, iv)
	output = cipher.encrypt(plaintext)
	return output


def xor(data, key):
	
	key = str(key)
	l = len(key)
	output_str = ""

	for i in range(len(data)):
		current = data[i]
		current_key = key[i % len(key)]
		output_str += chr(ord(current) ^ ord(current_key))
	
	return output_str


def printCiphertext(ciphertext):
	return '{ (char)0x' + ', (char)0x'.join(hex(ord(x))[2:] for x in ciphertext) + ' }'


def generateConfig(outputFilePath):

    fileConfigJsonPath = os.path.join(Path(__file__).parent, 'BeaconConfig.json')
    fileConfig = open(fileConfigJsonPath, 'r')
    config = fileConfig.read()

    fileEncryptPath = outputFilePath
    fileEncrypt = open(fileEncryptPath, 'w')

    fileClearPath = os.path.join(Path(__file__).parent, 'clearDef.h')
    fileClear = open(fileClearPath, 'r')
    
    fileClearContent = fileClear.read()
    config = xor(config, KEY_XOR)

    test = printCiphertext(config)
    fileClearContent = fileClearContent.replace("CONFIG", test)

    test = printCiphertext(KEY_XOR)
    fileClearContent = fileClearContent.replace("KEY_XOR", test)


    fileEncrypt.write(fileClearContent)

    fileEncrypt.close()

    return 


def main(argv):

    outputFilePath = "./cryptDef.hpp"

    opts, args = getopt.getopt(argv,"hb:o:",["output="])
    for opt, arg in opts:
            if opt == '-h':
                    print ('.. -o ./cryptDef.h')
                    sys.exit()
            elif opt in ("-o", "--output"):
                    outputFilePath = arg
    
    print('[+] Generate config:')

    generateConfig(outputFilePath)



if __name__ == "__main__":
    main(sys.argv[1:])

