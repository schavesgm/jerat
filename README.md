# JERAT

Program to fit correlation functions at different temperatures.

Before commiting any changes, please run,
```bash
make install
```

This action will set up a `git hook` that forces the following
commits to pass the tests defined in the folder `tests`
automatically.

## What does the code do ?
The code fits correlation functions for a given family (flavour
structure, gamma strucure, type of sources used...) automatically for
all the given temperatures provided. The code performs a hierarchy
fit through all the temperatures. It allows you to compare the same
fit for all temperatures.

The algorithm performed consists on:

1. We start with `M` files with different temperatures ordered from 
higher (`Nt` smaller) to lower (`Nt` larger).
2. We select the smaller `Nt` of all of them. 
3. For this `Nt` we calculate all the possible windows from the
   starting Euclidean time `t_1 = beg_window * Nt` to `t_2 = Nt / 2 -
   1`. 
4. We fit this value of `Nt` to all the windows defined in the last
   step. If a window is created by using `t_1,t_2`, we will fit from
   `[t_1 - t_2] U [Nt - t_2, Nt - t_1]` if the correlation function
   used is symmetric (`symm true` in the input file). The last value
   of `t_2` is constrained to be `N_t - a`. In the case of not having
   any symmetry, i.e, single exponentials, we will fit the data to
   the following window `[t_1,t_2]`. `t_2` can vary from `t_1 + 1` to
   `N_t - 1`. This second way is setup by using `symm false` is the 
   input file.
5. We fit the same windows for all the lowe temperatures (increasing
   `Nt`). This allows us to compare how the fitted parameters change
   with temperature in a controlled way.
6. We select the following value of `Nt` and go to step (1). We stop
   if `Nt` is equal to the last value of `Nt` provided (the largest).

In order to run the code you have to compile it. A `Makefile` is
provided,

```bash 
make 
```

Running the code is just a matter of using,

```bash 
./jerat input_file
```

---
Sergio Chaves García-Mascaraque -- COVID-19 times.
