
src = ../src/*.cpp
obj = *.o

gxx = g++
gxxflags = -Wall 
out = Horst

all: $(out)

$(out): $(obj)
	$(gxx) $(gxxflags) -o $(out) $(obj)
	
$(obj) : $(src)
	$(gxx) -c $(src)

clean:
	rm *.o $(out)
	clear

run: $(out)
	./$(out)
