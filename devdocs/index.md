Introduction
============
This is the developer documentation for the BEToolbox Extensions. These are experimental extensions that are not intended for production engineering work.

St. Venant Torsional Constant
-----------------------------
This tool computes the St. Venant torsional constant for precast prestressed girder bridge girders. 

    // shape = Build a shape
    Float64 min_element_size = 0.25;
    FDMeshGenerator mesh_generator(min_element_size,min_element_size);
    UniformFDMesh mesh;
    mesh_generator.GenerateMesh(shape,mesh);
    PrandtlMembrane membrane;
    Floa64 J = membrane.ComputeJ(mesh);

