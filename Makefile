#################################################
##              Makefile template              ##
#################################################

#     These special targets are called phony     #
.PHONY: all dist clean install uninstall dvi gcov build Qmakefile

build: ########################
	cmake -S . -B ./../build
	cmake --build ./../build

install: build ########################
	cp -rf ./../build/Viewer3D.app $(HOME)/Applications/

uninstall: ########################
	rm -rf $(HOME)/Applications/Viewer3D.app

############ Cleaning up unused files ############
clean: ########################
	rm -rf ./../build obj
	rm -rf *.o a.out *.gcda *.gcno report/ *.info *.cfg *.user* *.clang-format *.tgz
	rm -rf ./documentation/3DViewer

################## Style check ###################
style: build ########################
	cd ./../build;\
	make style

cppcheck: build ########################
	cd ./../build;\
	make cppcheck

################## Leaks check ###################
leaks: build
	leaks -atExit -- ./../build/Viewer3D.app/Contents/MacOS/Viewer3D

##############  Project archiving   ##############
dist: clean ########################
	tar -czf 3DViewer.tgz ./*

##############         Tests        ##############
test: build
	cd ./../build/_deps/assimp-build/bin;\
	./unit

##############     gcov report      ##############
gcov_report: test

##############     show instuction      ##############
dvi: ########################
	cd documentation;\
	makeinfo --html ./info.texi;\
	cp -rf src 3DViewer;\
	open 3DViewer/index.html

obj:
	git clone https://github.com/Astrodynamic/sample_obj.git obj