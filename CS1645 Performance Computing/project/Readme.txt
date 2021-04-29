with Berkeley UPC compiler installed from https://upc.lbl.gov/

$ upcc --network=smp -O -T 4 -o mmult mmult_par.upc
$ upcrun mmult

more imformation provided in report
