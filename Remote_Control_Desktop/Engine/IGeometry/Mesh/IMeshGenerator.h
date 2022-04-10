#ifndef IMESHGENERATOR_H
#define IMESHGENERATOR_H


#include "IMesh.h"


#include <functional>
#include <cstdint>


namespace IEngine
{



    //! Namespace with all mesh generation functions.
    namespace MeshGenerator
    {




        /**
        \brief Vertex modifier function interface.
        \param[in] u Specifies the interpolation factor U. This is in the range [0, 1].
        \param[in] v Specifies the interpolation factor V. This is in the range [0, 1].
        \return Interpolation factor which should be in the range [0, 1].
        \remarks This can be used for a couple of mesh generators, to adjust the final vertex position.
        \see TorusKnotDescriptor
        */
        using VertexModifier = std::function<float(float u, float v)>;

        /**
        \brief Function interface for an arbitrary R -> R^3 transformation.
        \param[in] t Specifies the curve progression. This is in the range [0, 1].
        \return 3D point which lies on the curve at the position 't'.
        \see CurveDescriptor
        */
        using CurveFunction = std::function<Vector3(float t)>;


        /* --- Descriptors --- */

        //! Descriptor structure for a cuboid (also cube) mesh.
        struct CuboidDescriptor
        {

            CuboidDescriptor( const Vector3& _size = Vector3(float(1.0)) ,
                              const Vector3i _segments = IMath::IVector3i(1, 1, 1),
                              bool _alternateGrid  = false):
               size(_size),
               segments(_segments) ,
               alternateGrid(_alternateGrid)
            {

            }

            //! Cuboid size. By default (1, 1, 1).
            Vector3  size  = Vector3(float(1.0));

            //! Cuboid segmentation. Each component will be clamped to [1, +inf). By default (1, 1, 1).
            IMath::IVector3i   segments   = IMath::IVector3i(1, 1, 1);

            //! Specifies whether the face grids are to be alternating or uniform. By default false.
            bool  alternateGrid   = false;
        };



        //! Descriptor structure for an ellispoid (also sphere) mesh.
        struct EllipsoidDescriptor
        {

            EllipsoidDescriptor( const Vector3& _radius = Vector3(float(0.5)),
                                 const Vector2i& _segments =  IMath::IVector2i(20, 10),
                                 bool  _alternateGrid = false) :
                radius(_radius),
                segments(_segments),
                alternateGrid(_alternateGrid)

            {

            }


            //! Radius in X, Y, and Z direction. By default (0.5, 0.5, 0.5).
            Vector3  radius  = Vector3(float(0.5));

            /**
            Segmentation in U (x component), and V (y component) direction.
            X component will be clamped to [3, +inf), Y component will be clamped to [2, +inf). By default (20, 10).
            */
            IMath::IVector2i   segments   = IMath::IVector2i(20, 10);

            //! Specifies whether the face grids are to be alternating or uniform. By default false.
            bool  alternateGrid   = false;
        };



        //! Descriptor structure for a cone mesh.
        struct ConeDescriptor
        {


            ConeDescriptor( const Vector2& _radius = Vector2(float(0.5)) ,
                            const float _height = float(1) ,
                            const Vector2i _mantleSegments  = Vector2i(20, 1),
                            const std::uint32_t  _coverSegments = 1,
                            bool _alternateGrid = false):
                radius(_radius),
                height(_height),
                mantleSegments(_mantleSegments),
                coverSegments(_coverSegments),
                alternateGrid(_alternateGrid)

            {

            }

            //! Cone radius in U (x component), and V (y component) direction. By default (0.5, 0.5).
            Vector2     radius   = Vector2(float(0.5));

            //! Cone height. By default 1.
            float        height   = float(1);

            //! Segmentation around the cone (x component), and height (y component). By default (20, 1).
            IMath::IVector2i   mantleSegments  = IMath::IVector2i(20, 1);

            //! Segmentation of the bottom cover. If 0, no bottom cover is generated. By default 1.
            std::uint32_t   coverSegments   = 1;

            //! Specifies whether the face grids are to be alternating or uniform. By default false.
            bool            alternateGrid   = false;
        };



        //! Descriptor structure for a cylinder mesh.
        struct CylinderDescriptor
        {

            CylinderDescriptor( const Vector2& _radius = Vector2(float(0.5)),
                                const float& _height   = float(1),
                                const Vector2i& _mantleSegments = IMath::IVector2i(20, 1) ,
                                const std::uint32_t&  _topCoverSegments    = 1,
                                const std::uint32_t&  _bottomCoverSegments = 1,
                                bool  _alternateGrid   = false) :
                    radius(_radius),
                    height(_height),
                    mantleSegments(_mantleSegments),
                    topCoverSegments( _topCoverSegments),
                    bottomCoverSegments(_bottomCoverSegments),
                    alternateGrid(_alternateGrid)

            {

            }



            //! Cylinder radius in U (x component), and V (y component) direction. By default (0.5, 0.5).
            Vector2     radius   = Vector2(float(0.5));

            //! Cylinder height. By default 1.
            float        height   = float(1);

            //! Segmentation around the cylinder (x component), and height (y component). By default (20, 1).
            IMath::IVector2i   mantleSegments      = IMath::IVector2i(20, 1);

            //! Segmentation of the top cover. If 0, no top cover is generated. By default 1.
            std::uint32_t   topCoverSegments    = 1;

            //! Segmentation of the bottom cover. If 0, no bottom cover is generated. By default 1.
            std::uint32_t   bottomCoverSegments = 1;

            //! Specifies whether the face grids are to be alternating or uniform. By default false.
            bool            alternateGrid       = false;
        };




        //! Descriptor structure for a torus-knot mesh (uses the curve generator).
        struct TorusKnotDescriptor
        {
            //! Radius of the torus ring in X, and Y direction. By default (0.25, 0.25, 0.25).
            Vector3     ringRadius      = Vector3(float(0.25));

            //! Radius of the inner tube. By default 0.125.
            float        tubeRadius      = float(0.125);

            //! Inner radius within the torus knot curve. By default 2.
            float        innerRadius     = float(2);

            /**
            \breif Number of loops within the torus knot. By default 2.
            \remarks This must be coprime to 'turns', otherwise the mesh will not be a valid torus knot.
            */
            std::uint32_t   loops           = 2;

            /**
            \breif Number of turns within the torus knot. By default 3.
            \remarks This must be coprime to 'loops', otherwise the mesh will not be a valid torus knot.
            */
            std::uint32_t   turns           = 3;

            /**
             \brief Segmentation in U (x component), and V (y component) direction.
             \remarks Each component will be clamped to [3, +inf). By default (256, 20).
            */
            IMath::IVector2i   segments        = IMath::IVector2i(256, 20);

            //! Specifies whether the face grids are to be alternating or uniform. By default false.
            bool            alternateGrid   = false;

            //! Vertex modifier to adjust the tube radius during mesh generation.
            VertexModifier  vertexModifier  = nullptr;
        };




        /* --- Global Functions --- */

         //================================================================+++++++++++++++++++++=//
        //! Generates a cuboid (also cube) mesh with the specified descriptor and appends the result to the specified output mesh.
        void GenerateMesh(const CuboidDescriptor& desc, IMesh& mesh);

        //! Generates and returns a new cuboid (also cube) mesh with the specified descriptor.
        IMesh GenerateMeshT(const CuboidDescriptor& desc);
         //================================================================+++++++++++++++++++++=//



        //================================================================+++++++++++++++++++++=//
        //! Generates an ellipsoid (also sphere) mesh with the specified descriptor and appends the result to the specified output mesh.
        void GenerateMesh(const EllipsoidDescriptor& desc, IMesh& mesh);

        //! Generates and returns a new ellipsoid (also sphere) mesh with the specified descriptor.
        IMesh GenerateMeshT(const EllipsoidDescriptor& desc);
        //================================================================+++++++++++++++++++++=//



        //================================================================+++++++++++++++++++++=//
        //! Generates a cone mesh with the specified descriptor and appends the result to the specified output mesh.
        void GenerateMesh(const ConeDescriptor& desc, IMesh& mesh);

        //! Generates and returns a new cone mesh with the specified descriptor.
        IMesh GenerateMeshT(const ConeDescriptor& desc);
        //================================================================+++++++++++++++++++++=//



        //================================================================+++++++++++++++++++++=//
        //! Generates a cylinder mesh with the specified descriptor and appends the result to the specified output mesh.
        void GenerateMesh(const CylinderDescriptor& desc, IMesh& mesh);

        //! Generates and returns a new cylinder mesh with the specified descriptor.
        IMesh GenerateMeshT(const CylinderDescriptor& desc);
        //================================================================+++++++++++++++++++++=//



    } // /namespace MeshGenerator




    class IMeshGenerate : public  IMesh
    {

    public:

        //! Descriptor structure for a cuboid (also cube) mesh.
        IMeshGenerate(MeshGenerator::CuboidDescriptor __descriptor_cuboid__) ;

        //! Descriptor structure for an ellispoid (also sphere) mesh.
        IMeshGenerate(MeshGenerator::EllipsoidDescriptor __descriptor_Ellipsoid__) ;

        //! Descriptor structure for a cone mesh.
        IMeshGenerate(MeshGenerator::ConeDescriptor __descriptor_Cone__) ;

        //! Descriptor structure for a cylinder mesh.
        IMeshGenerate(MeshGenerator::CylinderDescriptor __descriptor_Cylinder__) ;

        //! Descriptor structure for a torus-knot mesh (uses the curve generator).
        IMeshGenerate(MeshGenerator::TorusKnotDescriptor __descriptor_TorusKnot__) ;


    };




}

#endif // IMESHGENERATOR_H
