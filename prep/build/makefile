
gxx =
gxxflags =
cxxflags =
out =

src = ../src/*.cpp
obj = *.o



all: $(out)
	@echo Build war erfolgreich!!

$(out): $(obj)
	@echo ...Linke die Objektdateien zu einer Executable...
	@$(gxx) $(gxxflags) -o $(out) $(obj)
	@echo ...Objektdateien erfolgreich zu einer Executable gelinkt!!
	
$(obj) : $(src)
	@echo Beginne Build...
	@echo ...Kompiliere die Sourcedateien zu Objektdateien...
	@$(gxx) $(cxxflags) -c $(src)
	@echo ...Sourcedateien erfolgreich zu Objektdateien kompiliert!!

clean:
	@echo ...Lösche alle Objektdateien und die Executable...
	@rm -f *.o $(out)
	@echo ...Objektdateien und Executable erfolgreich gelöscht!!
	@clear

run: $(out)
	@echo Führe $(out) aus...
	@echo;
	@./$(out)
	@echo;
	@echo $(out) ausgeführt!!

scratch:
	@echo Löschen...
	rm -f *.o $(out)
	@echo Gelöscht!!
	@echo Kompilieren...
	@$(gxx) $(cxxflags) -c $(src)
	@echo Kompiliert!!
	@echo Linken...
	@$(gxx) $(gxxflags) -o $(out) $(obj)
	@echo Gelinkt!!
	@echo $(out) ausführen...
	@echo;
	@echo;
	@echo;
	@./$(out)
	@echo;
	@echo;
	@echo;
	@echo $(out) ausgeführt!!


