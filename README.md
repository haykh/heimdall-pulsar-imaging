# `Heimdall` pulsar imaging code

For details see [Hakobyan, Beskin (2014)](https://arxiv.org/pdf/1410.4581.pdf).

### Compilation & running
Use the following command to compile the code:
```bash
g++ --std=c++17 main.cpp io.cpp parameters.cpp algorithm.cpp -o main
```

Physical parameters for the simulation as well as numerical details, image resolution etc can be specified after the compilation in the so-called `input` file.

This will produce an executable `main`; you may now run it with: 
```bash
./main -i input -o tmp/output
```

Notice, that `-i` flag specifies the `input` file (which in fact can have any name) with the parameters, and `-o` flag specifies the output directory. The code will produce a series of `.dat` files in the specified output directory. These files contain the rows of `(a1, a2, intensity)`.

### Plotting
To plot the results simply run:
```bash
python plot.py tmp/output/ tmp/images/
```

We first specify the directory where the saved `.dat` files are stored (`tmp/output/`) then point the directory where plots will be saved (`tmp/images/`).
