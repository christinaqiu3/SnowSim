#include "mesh.h"
#include "halfedge.h"

mesh::mesh()
    : Drawable(nullptr), vertices(), halfedges(), faces()
{}

mesh::mesh(OpenGLContext* t)
    : Drawable(t), vertices(), halfedges(), faces()
{}

void mesh::create() {
    std::vector<GLuint> indices;
    std::vector<glm::vec4> positions;//1
    std::vector<glm::vec4> normals;//0
    std::vector<glm::vec4> colors;

    int totalV = 0;

    for (const auto& face : faces) {
        auto currentEdge = face->getIncidentHalfEdge();
        int verts = 0;
        //normal
        vertex borderVertex0 = *currentEdge->getNextVertex();
        vertex borderVertex1 = *currentEdge->getNextHalfEdge()->getNextVertex();
        vertex borderVertex2 = *currentEdge->getNextHalfEdge()->getNextHalfEdge()->getNextVertex();

        glm::vec3 vector1 = borderVertex1.getPosition() - borderVertex0.getPosition();
        glm::vec3 vector2 = borderVertex2.getPosition() - borderVertex0.getPosition();

        glm::vec3 faceNormal = glm::cross(vector2, vector1);
        glm::vec3 normal = glm::normalize(faceNormal);

        int icount = 0;
        do {

            positions.push_back(glm::vec4(currentEdge->getNextVertex()->position, 1.f));
            normals.push_back(glm::vec4(normal, 0.f));
            currentEdge = currentEdge->getNextHalfEdge();
            icount ++;
            verts ++;
            colors.push_back(glm::vec4(face->getColor(), 1.f));
        } while (currentEdge != face->getIncidentHalfEdge());


       helperTriangulate(indices, icount, totalV);
        totalV += verts;

    }

    count = indices.size();

    if(count > 0) {
    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec4), positions.data(), GL_STATIC_DRAW);

    generateNor();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    mp_context->glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec4), normals.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), GL_STATIC_DRAW);
    }
}

void mesh::helperTriangulate(std::vector<GLuint> &indices, int count, int totalV) {
    for (int i = 0; i < (int) count-2; i++) {
        indices.push_back(totalV);
        indices.push_back(i+ 1+totalV);
        indices.push_back(i+2+ totalV);
    }
}

// OBJ Import

static inline bool isSpace(const char c) { return (c == ' ') || (c == '\t'); }

static inline bool isNewLine(const char c) {
    return (c == '\r') || (c == '\n') || (c == '\0');
}


static inline int parseInt(const char *&token) {
    token += strspn(token, " \t");
    int i = atoi(token);
    token += strcspn(token, " \t\r");
    return i;
}

static inline glm::vec3 parseFloat(const char *&token) {
    QString q = QString(token);
    glm::vec3 vecs;
    QStringList qList = q.split(" ");
    int i = 0;
    for(auto s: qList) {
        vecs[i] = s.toFloat();
        i++;
    }
    return vecs;
}

std::string mesh::LoadObj(const char *filename) {
    halfedges.clear();
    vertices.clear();
    faces.clear();

    std::stringstream err;

    std::ifstream ifs(filename);
    if (!ifs) {
        err << "Cannot open file [" << filename << "]" << std::endl;
        return err.str();
    }
    std::istream &inStream = ifs;

    std::string name;

    int maxchars = 8192;             // Alloc enough size.
    std::vector<char> buf(maxchars); // Alloc enough size.
    std::map<std::pair<vertex*, vertex*>, halfedge*> vertexPairs;
    while (inStream.peek() != -1) {
        inStream.getline(&buf[0], maxchars);

        std::string linebuf(&buf[0]);

        // Trim newline '\r\n' or '\n'
        if (linebuf.size() > 0) {
            if (linebuf[linebuf.size() - 1] == '\n')
                linebuf.erase(linebuf.size() - 1);
        }
        if (linebuf.size() > 0) {
            if (linebuf[linebuf.size() - 1] == '\r')
                linebuf.erase(linebuf.size() - 1);
        }

        // Skip if empty line.
        if (linebuf.empty()) {
            continue;
        }

        // Skip leading space.
        const char *token = linebuf.c_str();
        token += strspn(token, " \t");

        assert(token);
        if (token[0] == '\0')
            continue; // empty line

        if (token[0] == '#')
            continue; // comment line

        // vertex
        if (token[0] == 'v' && isSpace((token[1]))) {
            token += 2;
            glm::vec3 vecs = parseFloat(token);
            vertices.push_back(new vertex(glm::vec3(vecs[0], vecs[1], vecs[2]), nullptr, vertices.size()));

            continue;
        }

        // face
        if (token[0] == 'f' && isSpace((token[1]))) {
            token += 2;
            std::vector<int> vertexIndices;
            while (!isNewLine(token[0])) {
                int vi = parseInt(token) - 1;
                vertexIndices.push_back(vi);
                size_t n = strspn(token, " \t\r");
                token += n;
            }

            std::vector<halfedge*> faceHalfEdges;
            for (size_t i = 0; i < vertexIndices.size(); ++i) {
                int v0 = vertexIndices[i];
                int v1 = vertexIndices[(i + 1) % vertexIndices.size()];

                halfedge* he = new halfedge(nullptr, nullptr, nullptr, vertices[v1], halfedges.size());
                vertices[v1]->setIncidentHalfEdge(he);
                faceHalfEdges.push_back(he);
                halfedges.push_back(he);
//                if (v0 > v1) {
//                    vertexPairs.insert(std::make_pair(std::make_pair(vertices[v0], vertices[v1]), halfedges.back()));
//                } else {
//                    vertexPairs.insert(std::make_pair(std::make_pair(vertices[v1], vertices[v0]), halfedges.back()));
//                }
            }

            glm::vec3 col = glm::vec3((double)rand()/(RAND_MAX), (double)rand()/(RAND_MAX), (double)rand()/(RAND_MAX));
            faces.push_back(new face(faceHalfEdges[0], col, faces.size()));

            for (size_t i = 0; i < faceHalfEdges.size(); ++i) {
                auto currentEdge = faceHalfEdges[i];
                auto nextEdge = faceHalfEdges[(i + 1) % faceHalfEdges.size()];

                currentEdge->setNextHalfEdge(nextEdge);

                // symmetric half-edges
                int v0 = vertexIndices[i];
                int v1 = vertexIndices[(i + 1) % faceHalfEdges.size()];
                std::pair<vertex*, vertex*> pair(vertices[v0], vertices[v1]);
                if (vertexPairs.count(pair) != 0) {
                    halfedge* symmEdge = vertexPairs.at(pair);
                    currentEdge->setSymHalfEdge(symmEdge);
                    symmEdge->setSymHalfEdge(currentEdge);
                    vertexPairs.erase(pair);
                } else {
                    vertexPairs[{vertices[v1], vertices[v0]}] = currentEdge;

//                    Vertex* a = vertices[v0];
//                    Vertex* b = vertices[v1];
//                    if (vertexPairs.count({a, b})) {
//                        halfedge* symEdge = vertexPairs[{a, b}];
//                        currentEdge->setSymHalfEdge(symEdge);
//                        vertexPairs.erase({a, b});
//                    } else {
//                        vertexPairs[{b, a}] = currentEdge;
//                    }
                }

                currentEdge->setIncidentFace(faces[faces.size()-1]);
            }

            continue;
        }

    }

    create();
    return err.str();
}

std::vector<vertex*> mesh::getVertss() {
    return vertices;
}

std::vector<halfedge*> mesh::getHalfEdgess() {
    return halfedges;
}

std::vector<face*> mesh::getFacess() {
    return faces;
}

