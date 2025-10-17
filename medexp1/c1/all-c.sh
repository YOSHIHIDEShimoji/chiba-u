#!/usr/bin/env zsh
source ~/.zshrc

for i in *.c; do
	echo "==> $i <=="
	c "$i"
	echo
done
