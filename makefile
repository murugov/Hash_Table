FLAGS = -D _DEBUG -ggdb3 -std=c++17 -O3 -m64 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations\
		-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts\
		-Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal\
		-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op\
		-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self\
		-Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2\
		-Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default\
		-Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros\
		-Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs\
		-Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging\
		-fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla -msse4.2 -mavx2 \

SANITIZE_FLAGS = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

COMMON_INCLUDES		= -I./COMMON/headers
VEC_INCLUDES		= -I./VECTOR
HT_INCLUDES 		= -I./HASH_TABLE


all: help

ht: main.cpp
	@echo "-----------------------------------------------------------------------------------------"
	g++ $(FLAGS) crc32_hash.o -o out main.cpp hash_func.cpp load_words_to_vec.cpp benchmark.cpp gen_hist.cpp $(VEC_INCLUDES) $(HT_INCLUDES) $(COMMON_INCLUDES)
	@echo "-----------------------------------------------------------------------------------------"

run-ht: ht
	./out


run: run-ht

clean:
	rm -f out

help:
	@echo "Available commands:"
	@echo ""
	@echo "  make ht                       - compile a hash table"
	@echo "  make run-ht                   - compile and run hash table"
	@echo "  make run                      - compile and run hash table"
	@echo ""
	@echo "  make clean                    - remove compiled programs"

.PHONY: ht run-ht run clean help