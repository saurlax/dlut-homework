set name=%1

mfile_to_poly.exe origin_surface_m/%name%.m
tetgen -Ypq1.1/14V origin_surface_m/%name%.poly
tetre_to_t.exe ../tetrahedral_mesh/%name%.t origin_surface_m/%name%.1.node origin_surface_m/%name%.1.ele


del origin_surface_m\%name%*.poly
del origin_surface_m\%name%.1.node
del origin_surface_m\%name%.1.ele
del origin_surface_m\%name%.1.edge
del origin_surface_m\%name%.1.face

VolumeViewer.exe ../tetrahedral_mesh/%name%.t
