$PSDocId: Document Version 1.0 for Runtime Library Version 3.3$

tuto0.c

This program is simple object viewer using GsTMDfast...() which is low level
interface of packet making.

The difference of GsSortObject4() and GsTMDfast..() is the level of handler.
GsSortObject4() cope with OBJECT level and GsTMDfast copes with polygon type
level.

SortTMDFlat() is the substitution of GsSortObject4() , but only for F3 and TF3
type polygons. If you use GsTMDfast() interface you must decode the TMD format
by yourself. 


