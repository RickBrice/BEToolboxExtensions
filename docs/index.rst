====================
BEToolbox Extensions
====================

This project implements experimental extensions to BridgeLink::BEToolbox. These extensions are not intended for production engineering work. They are partially developed and have not been fully validated.

Moment Curvature of Circular Columns
------------------------------------
This extension computes the moment curvature response of circular columns with bonded and unbonded longitudinal reinforcement and unbonded prestressed tendons.

St. Venant Torsional Constant
-----------------------------
This extension computes the St. Venant Torsional Constant for precast, prestressed concrete bridge girders. Calculations are carried out by the finite difference method solving Prandtl's membrane analogy.

API
---
This is the developer documentation for the BEToolbox Extensions. These are experimental extensions that are not intended for production engineering work.

Example
=======
This tool computes the St. Venant torsional constant for precast prestressed girder bridge girders. 

    // shape = Build a shape
    Float64 min_element_size = 0.25;
    FDMeshGenerator mesh_generator(min_element_size,min_element_size);
    UniformFDMesh mesh;
    mesh_generator.GenerateMesh(shape,mesh);
    PrandtlMembrane membrane;
    Floa64 J = membrane.ComputeJ(mesh);



Other Experemental Aspects
--------------------------
The other experimental aspects of this project are

#. Experiment with generating documentation using reSTructured text
#. Generation of end user documentation with Sphinx and Read the Docs


Contents:

.. toctree::
   :maxdepth: 3
   
   m3c
   svt
   

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

