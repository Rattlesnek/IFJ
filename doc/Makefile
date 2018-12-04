dokumentace.pdf: dokumentace.ps
	ps2pdf dokumentace.ps
dokumentace.ps: dokumentace.dvi
	dvips -t -sPAPERSIZE=a4 dokumentace.dvi
dokumentace.dvi: dokumentace.tex
	latex dokumentace.tex
	latex dokumentace.tex
