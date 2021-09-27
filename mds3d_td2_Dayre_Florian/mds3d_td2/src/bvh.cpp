
#include "bvh.h"
#include "mesh.h"
#include <iostream>

void BVH::build(const Mesh* pMesh, int targetCellSize, int maxDepth)
{
    // store a pointer to the mesh
    m_pMesh = pMesh;

    // allocate the root node
    m_nodes.resize(1);

    if(m_pMesh->nbFaces() <= targetCellSize) { // only one node
        m_nodes[0].box = pMesh->AABB();
        m_nodes[0].first_face_id = 0;
        m_nodes[0].is_leaf = true;
        m_nodes[0].nb_faces = m_pMesh->nbFaces();
        m_faces.resize(m_pMesh->nbFaces());
        for(int i=0; i<m_pMesh->nbFaces(); ++i)
        {
            m_faces[i] = i;
        }
    }else{
        // reserve space for other nodes to avoid multiple memory reallocations
        m_nodes.reserve( std::min<int>(2<<maxDepth, std::log(m_pMesh->nbFaces()/targetCellSize) ) );

        // compute centroids and initialize the face list
        m_centroids.resize(m_pMesh->nbFaces());
        m_faces.resize(m_pMesh->nbFaces());
        for(int i=0; i<m_pMesh->nbFaces(); ++i)
        {
            m_centroids[i] = (m_pMesh->vertexOfFace(i, 0).position + m_pMesh->vertexOfFace(i, 1).position + m_pMesh->vertexOfFace(i, 2).position)/3.f;
            m_faces[i] = i;
        }

        // recursively build the BVH, starting from the root node and the entire list of faces
        buildNode(0, 0, m_pMesh->nbFaces(), 0, targetCellSize, maxDepth);
    }
}

bool BVH::intersect(const Ray& ray, Hit& hit) const
{
    // compute the intersection with the root node
    float tMin, tMax;
    Normal3f n;

    // TODO
    // vérifier si on a bien une intersection (en fonction de tMin, tMax, et hit.t()), et si oui appeler intersecNode...
    if(::intersect(ray, m_nodes[0].box, tMin, tMax, n));
    {
        intersectNode(0, ray, hit);
        return true;
    }

    return false;
}

bool BVH::intersectNode(int nodeId, const Ray& ray, Hit& hit) const
{
    // TODO, deux cas: soit mNodes[nodeId] est une feuille (il faut alors intersecter les triangles du noeud),
    // soit c'est un noeud interne (il faut visiter les fils (ou pas))
    Node node = m_nodes[nodeId];
    float tMin, tMax;
    Normal3f n;
    Hit hit1;
    if(node.is_leaf == true){
        for (int i = 0; i < m_nodes[nodeId].nb_faces; i++)
        {
            m_pMesh->intersectFace(ray, hit1, m_faces[m_nodes[nodeId].first_face_id+i]); 
            
            if(hit1.t() < hit.t()){
                hit.setT(hit1.t());
                hit.setNormal(hit1.normal());
                hit.setShape(hit1.shape());
            }     
        }
    }
    else
    {
        Node leftNode = m_nodes[node.first_child_id];
        Node rightNode = m_nodes[node.first_child_id+1];

        if(::intersect(ray, leftNode.box, tMin, tMax, n))
        {
            intersectNode(m_nodes[nodeId].first_child_id, ray, hit);
        }
        if(::intersect(ray, rightNode.box, tMin, tMax, n))
        {
            intersectNode(m_nodes[nodeId].first_child_id+1, ray, hit);
        }
    }
    return true;
}

/** Sorts the faces with respect to their centroid along the dimension \a dim and spliting value \a split_value.
  * \returns the middle index
  */
int BVH::split(int start, int end, int dim, float split_value)
{
    int l(start), r(end-1);
    while(l<r)
    {
        // find the first on the left
        while(l<end && m_centroids[l](dim) < split_value) ++l;
        while(r>=start && m_centroids[r](dim) >= split_value) --r;
        if(l>r) break;
        std::swap(m_centroids[l], m_centroids[r]);
        std::swap(m_faces[l], m_faces[r]);
        ++l;
        --r;
    }
    return m_centroids[l][dim]<split_value ? l+1 : l;
}

void BVH::buildNode(int nodeId, int start, int end, int level, int targetCellSize, int maxDepth)
{
    // sleep(1);
    Node& node = m_nodes[nodeId];
    node.nb_faces = 0;

    // étape 1 : calculer la boite englobante des faces indexées de m_faces[start] à m_faces[end]
    // (Utiliser la fonction extend de Eigen::AlignedBox3f et la fonction mpMesh->vertexOfFace(int) pour obtenir les coordonnées des sommets des faces)
    Eigen::AlignedBox3f box;
    for(int i=start; i<end; i++){
        Point3f A = m_pMesh->vertexOfFace(m_faces[i] ,0).position;
        Point3f B = m_pMesh->vertexOfFace(m_faces[i], 1).position;
        Point3f C = m_pMesh->vertexOfFace(m_faces[i], 2).position;
        box.extend(A);
        box.extend(B);
        box.extend(C);
        node.nb_faces++;
    }

    node.box = box;
    // étape 2 : déterminer si il s'agit d'une feuille (appliquer les critères d'arrêts)

    // Si c'est une feuille, finaliser le noeud et quitter la fonction
    if(level == maxDepth || node.nb_faces <= targetCellSize)
    {
        node.is_leaf = true;
        node.first_face_id = start;
        return;
    }

    // Si c'est un noeud interne :

    // étape 3 : calculer l'index de la dimension (x=0, y=1, ou z=2) et la valeur du plan de coupe
    // (on découpe au milieu de la boite selon la plus grande dimension)
    Point3f dim1 = box.sizes();
    
    int dim;
    float split_value;

    if(dim1.x() > dim1.y() && dim1.x() > dim1.z()){
        dim = 0;
    }
    else if(dim1.y() > dim1.x() && dim1.y() > dim1.z()){
        dim = 1;
    }
    else if(dim1.z() > dim1.x() && dim1.z() > dim1.y()){
        dim = 2;
    }
    split_value = box.min()[dim] + box.sizes()[dim] / 2;


        
    // étape 4 : appeler la fonction split pour trier (partiellement) les faces et vérifier si le split a été utile
    int indice = split(start, end, dim, split_value);
    if(indice == start || indice == end){
        node.is_leaf = true;
        node.first_face_id = start;
        return;
    }

    // étape 5 : allouer les fils, et les construire en appelant buildNode...
    m_nodes.resize(m_nodes.size()+2);
        
    Node nodeLeft;
    int nodeLeftId = m_nodes.size()-2;
    m_nodes[nodeLeftId] = nodeLeft;

    Node nodeRight;
    int nodeRightId = m_nodes.size()-1;
    m_nodes[nodeRightId] = nodeRight;

    node.first_child_id = nodeLeftId;
    node.is_leaf = false;
    m_nodes[nodeId] = node;

    level += 1;
    buildNode(nodeLeftId, start, indice, level, targetCellSize, maxDepth);
    buildNode(nodeRightId, indice++, end, level, targetCellSize, maxDepth);
}
