import re
import sys
from os.path import join, isfile



sys.argv.pop(0)
out_corpus = sys.argv.pop(0)
out_vocab = sys.argv.pop(0)
corp = open(out_corpus, "w")
tab = {}

filenames = sys.argv

for filename in filenames:
	if not isfile(filename):
		print("No such filename "+filename)
		sys.exit(1)

	print("Making corpus from "+filename+" into "+out_corpus)

	f = open(filename)
	lines = f.readlines()
	for line in lines:
		line = line.strip()
		words = re.split("\t", line)
		if len(words)>=3:
			vocab_word = words[2]
			corp.write(vocab_word+"\n")
			tab[vocab_word] = 1


vocab = open(out_vocab,"w")
vocab.write("</s>\n")
for vocab_word in tab:
	vocab.write(vocab_word+"\n")
	