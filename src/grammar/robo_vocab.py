import re
import sys
from os.path import join, isfile
#import nltk


sys.argv.pop(0)
#out_corpus = sys.argv.pop(0)
out_vocab = sys.argv.pop(0)
tab = {}

filenames = sys.argv

for filename in filenames:
	if not isfile(filename):
		print("No such filename "+filename)
		sys.exit(1)

	print("Making vocab from "+filename+" into "+out_vocab)

	f = open(filename)
	lines = f.readlines()
	for line in lines:
		line = line.strip()
		tokens = line.split(" ")
		for token in tokens:
			if re.search(r'<[^<>]+>', token):
				pass
			else:
				token = re.sub(r'[^\w]+', '', token.upper())
				if len(token)>0:
					tab[token]=1
						#corp.write(token+"\n")
			
vocab = open(out_vocab,"w")
vocab.write("</s>\n")
for vocab_word in tab:
	vocab.write(vocab_word+"\n")
	