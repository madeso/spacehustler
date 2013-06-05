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
    | aiProcess_FlipUVs
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

    /** @todo add parsing of nodes to the mesh so we could
    dynamically animate some rotors, wings etc. for example
     */

    if (scene->HasMeshes() == false) {
      throw "Scene is missing meshes";
    }

    for (unsigned int materialId = 0; materialId < scene->mNumMaterials;
         ++materialId) {
      const aiMaterial* mat = scene->mMaterials[materialId];
      if (mat->GetTextureCount(aiTextureType_DIFFUSE) <= 0) {
        throw "Missing texture";
      }

      aiString path;
      mat->GetTexture(aiTextureType_DIFFUSE, 0, &path);
      ret.materials.push_back(path.C_Str());
    }

    for (unsigned int meshid = 0; meshid < scene->mNumMeshes; ++meshid) {
      const aiMesh* mesh = scene->mMeshes[meshid];
      MeshPart part;
      part.material = mesh->mMaterialIndex;
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
          part.addPoint(vertex.x, vertex.y, vertex.z, u, v);
        }
      }
      ret.parts.push_back(part);
    }
    return ret;
  }

  // http://assimp.sourceforge.net/howtoBasicShapes.html
  Mesh LoadFromString(const std::string& nff, const std::string& format) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFileFromMemory(nff.c_str(),
                           nff.length() + 1, kAssimpFlags, format.c_str());
    if (!scene) {
      throw importer.GetErrorString();
    }
    return ConvertScene(scene);
  }

  const std::string kFormatNff = "nff";
  const std::string kFormatObj = "obj";
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
  return LoadFromString(ss.str(), kFormatNff);
}

Mesh CreateSphere(float size) {
  std::ostringstream ss;
  ss << "shader texture.png" << std::endl << "s 0 0 0 " << size;
  return LoadFromString(ss.str(), kFormatNff);
}

Mesh CreateBox(float width, float height, float depth) {
  const float x = width / 2;
  const float y = height / 2;
  const float z = depth / 2;
  std::ostringstream ss;
  ss << "v " << -x << " " << " " << -y << " " << -z << std::endl
     << "v " << -x << " " << " " << -y << " " <<  z << std::endl
     << "v " << -x << " " << " " <<  y << " " << -z << std::endl
     << "v " << -x << " " << " " <<  y << " " <<  z << std::endl
     << "v " <<  x << " " << " " << -y << " " << -z << std::endl
     << "v " <<  x << " " << " " << -y << " " <<  z << std::endl
     << "v " <<  x << " " << " " <<  y << " " << -z << std::endl
     << "v " <<  x << " " << " " <<  y << " " <<  z << std::endl
     << "" << std::endl
     << "vn 0 0 -1" << std::endl
     << "vn 0 0 1" << std::endl
     << "vn -1 0 0" << std::endl
     << "vn 1 0 0" << std::endl
     << "vn 0 1 0" << std::endl
     << "vn 0 -1 0" << std::endl
     << "vn 0 0 -1" << std::endl
     << "vn 0 -1 0" << std::endl
     << "vn -1 0 0" << std::endl
     << "vn 0 0 1" << std::endl
     << "vn 0 -1 0" << std::endl
     << "vn -1 0 0" << std::endl
     << "vn 0 0 -1" << std::endl
     << "vn 0 1 0" << std::endl
     << "vn -1 0 0" << std::endl
     << "vn 0 0 1" << std::endl
     << "vn 0 1 0" << std::endl
     << "vn -1 0 0" << std::endl
     << "vn 0 0 -1" << std::endl
     << "vn 0 -1 0" << std::endl
     << "vn 1 0 0" << std::endl
     << "vn 0 0 1" << std::endl
     << "vn 0 -1 0" << std::endl
     << "vn 1 0 0" << std::endl
     << "vn 0 0 -1" << std::endl
     << "vn 0 1 0" << std::endl
     << "vn 1 0 0" << std::endl
     << "vn 0 0 1" << std::endl
     << "vn 0 1 0" << std::endl
     << "vn 1 0 0" << std::endl
     << "" << std::endl
     << "vt 0 0" << std::endl
     << "vt 0 1" << std::endl
     << "vt 1 1" << std::endl
     << "vt 1 0" << std::endl
     << "" << std::endl
     << "f 3/1/13 7/2/25 5/3/19 1/4/7" << std::endl
     << "f 6/1/22 8/2/28 4/3/16 2/4/10" << std::endl
     << "f 2/1/12 4/2/18 3/3/15 1/4/9" << std::endl
     << "f 7/1/27 8/2/30 6/3/24 5/4/21" << std::endl
     << "f 4/1/17 8/2/29 7/3/26 3/4/14" << std::endl
     << "f 5/1/20 6/2/23 2/3/11 1/4/8" << std::endl;

  return LoadFromString(ss.str(), kFormatObj);
}
