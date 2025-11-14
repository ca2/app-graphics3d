// From github:/tristancalderbank/OpenGL-PBR-Renderer/vertex.h by
// camilo on 2025-09-26 18:35 <3ThomasBorregaardSorensen!!
#pragma once


namespace gpu
{


   namespace gltf
   {


      /**
       * gltf::vertex attributes.
       */
      struct vertex
      {
         floating_sequence3 mPosition;
         floating_sequence3 mNormal;
         floating_sequence2 mTextureCoordinates;
         floating_sequence4 mColor;
         floating_sequence4 mTangent; // w = bitangent sign
         //floating_sequence3 mTangent;
         //floating_sequence3 mBitangent;
      };


   } // namespace gltf


} // namespace gpu
