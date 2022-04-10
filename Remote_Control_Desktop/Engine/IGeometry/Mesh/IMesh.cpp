#include "IMesh.h"
#include "IMeshGenerator.h"


namespace IEngine
{

using namespace MeshGenerator;


IMeshGenerate::IMeshGenerate(MeshGenerator::CuboidDescriptor __descriptor_cuboid__)
{
    MeshGenerator::GenerateMesh( __descriptor_cuboid__ , *this );
}

IMeshGenerate::IMeshGenerate(MeshGenerator::EllipsoidDescriptor __descriptor_Ellipsoid__)
{
    MeshGenerator::GenerateMesh( __descriptor_Ellipsoid__ , *this );
}

IMeshGenerate::IMeshGenerate(MeshGenerator::ConeDescriptor __descriptor_Cone__)
{
    MeshGenerator::GenerateMesh( __descriptor_Cone__ , *this );
}

IMeshGenerate::IMeshGenerate(MeshGenerator::CylinderDescriptor __descriptor_Cylinder__)
{
     MeshGenerator::GenerateMesh( __descriptor_Cylinder__, *this );
}



}
