// Euphoria - Copyright (c) Gustav

#include "euphoria/mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <sstream>

namespace {
  const unsigned int kAssimpFlags =
    aiProcess_CalcTangentSpace
    | aiProcess_Triangulate
    | aiProcess_SortByPType
    | aiProcess_MakeLeftHanded
    | aiProcess_GenUVCoords
    | aiProcess_TransformUVCoords
    | aiProcess_OptimizeMeshes
    | aiProcess_RemoveRedundantMaterials
    | aiProcess_PreTransformVertices
    | aiProcess_ImproveCacheLocality
    | aiProcess_FindDegenerates
    | aiProcess_JoinIdenticalVertices
    | aiProcess_ValidateDataStructure
    | aiProcess_GenSmoothNormals
    | aiProcess_FindInvalidData;

  Mesh ConvertScene(const aiScene* scene) {
    Mesh ret;
    if (scene->HasMeshes() == false) {
      throw "Scene is missing meshes";
    }
    for (unsigned int meshid = 0; meshid < scene->mNumMeshes; ++meshid) {
      const aiMesh* mesh = scene->mMeshes[meshid];
      for (unsigned int faceid = 0; faceid < mesh->mNumFaces; ++faceid) {
        const aiFace& face = mesh->mFaces[faceid];
        for (unsigned vertid = 0; vertid < 3; ++vertid) {
          const unsigned int index = face.mIndices[vertid];
          const aiVector3D& vertex = mesh->mVertices[index];
          float u = 0;
          float v = 0;
          if (mesh->HasTextureCoords(0)) {
            const aiVector3D uv = mesh->mTextureCoords[0][index];
            u = uv.x;
            v = uv.y;
          }
          ret.addPoint(vertex.x, vertex.y, vertex.z, u, v);
        }
      }
    }
    return ret;
  }

  // http://assimp.sourceforge.net/howtoBasicShapes.html
  Mesh LoadFromNffString(const std::string& nff) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFileFromMemory(nff.c_str(),
                           nff.length() + 1, kAssimpFlags, "nff");
    if (!scene) {
      throw importer.GetErrorString();
    }
    return ConvertScene(scene);
  }
}  // namespace

Mesh LoadMesh(const std::string& path) {
  Assimp::Importer importer;

  const aiScene* scene = importer.ReadFile(path, kAssimpFlags);
  if (!scene) {
    throw importer.GetErrorString();
  }
  return ConvertScene(scene);
}

Mesh CreateCube(float size) {
  std::ostringstream ss;
  ss << "shader texture.png" << std::endl << "hex 0 0 0 " << size;
  return LoadFromNffString(ss.str());
}

Mesh CreateSphere(float size) {
  std::ostringstream ss;
  ss << "shader texture.png" << std::endl << "s 0 0 0 " << size;
  return LoadFromNffString(ss.str());
}
