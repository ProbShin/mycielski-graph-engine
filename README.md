# mycielski-graph-engine
generate, do test on mycielski graph


# introduction to Mycielski Graphs
[MycielskiGraph](http://mathworld.wolfram.com/MycielskiGraph.html)
[MycielskiGraph](https://en.wikipedia.org/wiki/Mycielskian)

# compile 
`
$make
`

# help
`
$./MCEngine.out
`

# usage
`
fprintf(stderr, "\nUsage: ./MCEngin.out -s <propagate steps> [-f <seeds_files>] [-r] [-o <output_files>]\n"
                        "\n"
                        "### Readme\n"
                        "     This program 'MCEngine.out' a.k.a 'Mycielski Graph Engine' would generate Mycielski graphs accoridng different policies/algorihtm/heuristics, such as *standard Mycielski graphs with/without specified seed* (each steps duplicates nodes, connectes corresponding nodes's neigbhors, then adding extral node connecting all the duplicated nodes.); *random Mycielski graph* with/without specified seed (each step duplicate nodes, randomly connected corresponding nodes's neighbors. then adding extral node connecting all the duplicated nodes.). The initial graph is the seed graph, default graph is K2. In case of problems, email cheng172@purdue.edu\n"
                        "\n"
                        "     -s <s>       : The graph will be propagate s steps.\n"
                        "     -f <in_file> : The input seed file. \n"
                        "     -r           : With this flag, the random Mycielski rules will replace standard Mycielski rules.\n"
                        "     -o <out_file>: The output seed file. \n"
                        "\n"
                        "### Examples:\n"
                        "# standard mycielski M5 (K2 propagate 3 steps) graphs:\n"
                        "     $./MCEngin.out -s 3\n"
                        "\n"
                        "# standard mycielski graph propagate 2 steps using seed graphs:\n"
                        "     $./MCEngin.out -s 2 -f bcsstk01.mtx\n"
                        "\n"
                        "# random mycielski graph propagate 1 steps using seed graphs, and the specify output filename:\n"
                        "     $./MCEngin.out -s 1 -o rnd_bcsstk01_mc1.mtx -f bcsstk01.mtx -r\n"
                        "\n"
               );

`
