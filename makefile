CXX      = clang++
SANITIZE = -fsanitize=address -fsanitize=undefined-trap -fsanitize-undefined-trap-on-error
COVERAGE = -fprofile-instr-generate -fcoverage-mapping
OPTS     = -std=c++17 $(SANITIZE) $(COVERAGE) -Weverything -Wno-c++98-compat -Wno-padded -Wno-poison-system-directories
XCRUN    = xcrun

.PHONY : all
all : rollingmean.coverage fcmp.coverage

%.coverage : %.profdata
	$(XCRUN) llvm-cov show $*.unittest -instr-profile=$< $*.cpp > $@
	! grep " 0|" $@
	echo PASS $@

%.profdata : %.profraw
	$(XCRUN) llvm-profdata merge -sparse $< -o $@

%.profraw : %.unittest
	LLVM_PROFILE_FILE=$@ ./$<

%.unittest : test_%.cpp %.cpp
	$(CXX) $(OPTS) $^ -o $@

rollingmean.unittest : fcmp.cpp

.PHONY : clean
clean :
	rm -rf *.coverage *.profdata *.profraw *.unittest*
