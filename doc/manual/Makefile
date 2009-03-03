
SECTIONS = preamble.tex

all: clean accounting-guide.pdf

clean:
	-rm -f *.aux *.dvi *.log *.idx *.ind *.toc *.pdf

accounting-guide.pdf: accounting-guide.dvi
	dvipdf accounting-guide

accounting-guide.dvi: accounting-guide.tex
	latex accounting-guide
	latex accounting-guide

admin-guide.pdf : admin-guide.dvi
	   dvipdf admin-guide

admin-guide.dvi : admin-guide.tex
	   latex admin-guide
	   latex admin-guide

#manual.ind : manual.idx
#	   makeindex manual
#manual.idx : *.tex
#	   latex manual


# vim: noet sts=0 sw=4 ts=4
