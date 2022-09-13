all: mkdirectory build 

FLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE

build:  main.o work_with_text.o log_info.o generals.o
	g++ obj/main.o obj/work_with_text.o obj/log_info.o obj/generals.o -o hamlet

main.o: main.cpp
	g++ -c main.cpp -o obj/main.o $(FLAGS)

work_with_text.o: work_with_text.cpp
	g++ -c work_with_text.cpp -o obj/work_with_text.o $(FLAGS)

log_info.o: log_info.cpp
	g++ -c log_info.cpp -o obj/log_info.o $(FLAGS)

generals.o: Generals_func/generals.cpp
	g++ -c Generals_func/generals.cpp -o obj/generals.o $(FLAGS)

.PHONY: cleanup mkdirectory

mkdirectory:
	mkdir -p obj

cleanup:
	rm *.o hamlet