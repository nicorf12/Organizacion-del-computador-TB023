CIRCUIT_FILE = TallerLogica-Combinatorios-individual.circ
EXPECTED_OUTPUT_FILE = salida_catedra.txt
FLAGS := --toplevel-circuit "verificador" -tty table
LOGISIM := logisim-evolution
SALIDA := output.txt

.PHONY: all verificador clean
all: verificador

verificador:
	@echo "EJECUTANDO PRUEBAS"
	@$(LOGISIM) $(FLAGS) $(CIRCUIT_FILE) > $(SALIDA)
	@diff output.txt $(EXPECTED_OUTPUT_FILE) && echo "La solución produce los resultados esperados" || echo "La solución no produce los resultados esperados"
	@echo "RESPUESTA DE 3.3: inversor_4 toma una entrada A, invierte cada bit y usando un sumador hardcodeado con B en 1 logramos tener el complemento a 2 del numero A. Entonces la salida leera el complemento a 2 de A o A dependiendo de lo que la entrada de control INV diga. 1 seria el complemento a dos de A y 0 seria A."
clean:
	rm -f $(SALIDA)

