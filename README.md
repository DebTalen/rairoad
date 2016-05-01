"# railroad" 

Just implementation of search of the shortest path between two or more railroad stations, uses dijkstra's algorithm.
Finds the fastest or the cheapest way (or ways) between two stations (or from one to all others).

Usage:

          railroad (Departure Station Id) (Arrival Station Id) (Priority)
Priority can only be "price" or "time" without quotes.
Arrival Station Id can also be "all" without quotes.

Required xml file, the same format as in example data.xml, and the TinyXml lib.
Feel free to use it.
