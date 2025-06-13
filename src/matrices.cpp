#include "../include/matrices.h"

glm::mat4 Matrix(
    float m00, float m01, float m02, float m03, // LINHA 1
    float m10, float m11, float m12, float m13, // LINHA 2
    float m20, float m21, float m22, float m23, // LINHA 3
    float m30, float m31, float m32, float m33  // LINHA 4
)
{
    return glm::mat4(
        m00, m10, m20, m30, // COLUNA 1
        m01, m11, m21, m31, // COLUNA 2
        m02, m12, m22, m32, // COLUNA 3
        m03, m13, m23, m33  // COLUNA 4
    );
}

// Matriz identidade.
glm::mat4 Matrix_Identity()
{
    return Matrix(
        1.0f , 0.0f , 0.0f , 0.0f , // LINHA 1
        0.0f , 1.0f , 0.0f , 0.0f , // LINHA 2
        0.0f , 0.0f , 1.0f , 0.0f , // LINHA 3
        0.0f , 0.0f , 0.0f , 1.0f   // LINHA 4
    );
}


glm::mat4 Matrix_Translate(float tx, float ty, float tz)
{
    return Matrix(
        1.0f , 0.0f , 0.0f , tx ,  // LINHA 1
        0.0f , 1.0f , 0.0f , ty ,  // LINHA 2
        0.0f , 0.0f , 1.0f , tz ,  // LINHA 3
        0.0f , 0.0f , 0.0f , 1.0f    // LINHA 4
    );
}

glm::mat4 Matrix_Scale(float sx, float sy, float sz)
{
    return Matrix(
        sx , 0.0f , 0.0f , 0.0f ,  // LINHA 1
        0.0f , sy , 0.0f , 0.0f ,  // LINHA 2
        0.0f , 0.0f , sz , 0.0f ,  // LINHA 3
        0.0f , 0.0f , 0.0f , 1.0f    // LINHA 4
    );
}

glm::mat4 Matrix_Rotate_X(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return Matrix(
        1.0f , 0.0f , 0.0f , 0.0f ,  // LINHA 1
        0.0f , c , -s , 0.0f ,  // LINHA 2
        0.0f , s , c , 0.0f ,  // LINHA 3
        0.0f , 0.0f , 0.0f , 1.0f    // LINHA 4
    );
}

glm::mat4 Matrix_Rotate_Y(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return Matrix(
        c , 0.0f , s , 0.0f ,  // LINHA 1
        0.0f , 1.0f , 0.0f , 0.0f ,  // LINHA 2
        -s , 0.0f , c , 0.0f ,  // LINHA 3
        0.0f , 0.0f , 0.0f , 1.0f    // LINHA 4
    );
}

glm::mat4 Matrix_Rotate_Z(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return Matrix(
        c , -s , 0.0f , 0.0f ,  // LINHA 1
        s , c , 0.0f , 0.0f ,  // LINHA 2
        0.0f , 0.0f , 1.0f , 0.0f ,  // LINHA 3
        0.0f , 0.0f , 0.0f , 1.0f    // LINHA 4
    );
}

float norm(glm::vec4 v)
{
    float vx = v.x;
    float vy = v.y;
    float vz = v.z;

    return sqrt(vx*vx + vy*vy + vz*vz);
}

glm::mat4 Matrix_Rotate(float angle, glm::vec4 axis)
{
    float c = cos(angle);
    float s = sin(angle);

    glm::vec4 v = axis / norm(axis);

    float vx = v.x;
    float vy = v.y;
    float vz = v.z;

    return Matrix(
        // PREENCHA AQUI A MATRIZ DE ROTA��O (3D) EM TORNO DO EIXO v EM COORD.
        // HOMOG�NEAS, UTILIZANDO OS PAR�METROS vx, vy, vz, c e s (F�RMULA DE RODRIGUES)
         c + vx*vx*(1-c)      , vx*vy*(1-c) - vz*s , vx*vz*(1-c) + vy*s , 0.0f,
        vy*vx*(1-c) + vz*s   , c + vy*vy*(1-c)    , vy*vz*(1-c) - vx*s , 0.0f,
        vz*vx*(1-c) - vy*s   , vz*vy*(1-c) + vx*s , c + vz*vz*(1-c)    , 0.0f,
        0.0f                 , 0.0f               , 0.0f               , 1.0f
    );
}

glm::vec4 crossproduct(glm::vec4 u, glm::vec4 v)
{
    float u1 = u.x;
    float u2 = u.y;
    float u3 = u.z;
    float v1 = v.x;
    float v2 = v.y;
    float v3 = v.z;

    return glm::vec4(
        u2*v3 - u3*v2, // Primeiro coeficiente
        u3*v1 - u1*v3, // Segundo coeficiente
        u1*v2 - u2*v1,
        0.0f // w = 0 para vetores.
    );
}

// Produto escalar entre dois vetores u e v definidos em um sistema de
// coordenadas ortonormal.
float dotproduct(glm::vec4 u, glm::vec4 v)
{
    float u1 = u.x;
    float u2 = u.y;
    float u3 = u.z;
    float u4 = u.w;
    float v1 = v.x;
    float v2 = v.y;
    float v3 = v.z;
    float v4 = v.w;

    if ( u4 != 0.0f || v4 != 0.0f )
    {
        fprintf(stderr, "ERROR: Produto escalar n�o definido para pontos.\n");
        std::exit(EXIT_FAILURE);
    }

    return u1*v1 + u2*v2 + u3*v3;
}

// Matriz de mudan�a de coordenadas para o sistema de coordenadas da C�mera.
glm::mat4 Matrix_Camera_View(glm::vec4 position_c, glm::vec4 view_vector, glm::vec4 up_vector)
{
    glm::vec4 w = -view_vector;
    glm::vec4 u = crossproduct(up_vector, w);

    // Normalizamos os vetores u e w
    w = w / norm(w); // view_vector
    u = u / norm(u);

    glm::vec4 v = crossproduct(w,u); //up vector

    glm::vec4 origin_o = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    float ux = u.x;
    float uy = u.y;
    float uz = u.z;
    float vx = v.x;
    float vy = v.y;
    float vz = v.z;
    float wx = w.x;
    float wy = w.y;
    float wz = w.z;

    glm::vec4 vc = position_c - origin_o;

    return Matrix(
        // PREENCHA AQUI A MATRIZ DE MUDAN�A DE SISTEMA DE COORDENADAS (3D)
        // PARA AS COORDENADAS DE C�MERA (MATRIZ VIEW HOMOG�NEA), UTILIZANDO
        // OS PAR�METROS ux,uy,uz, vx,vy,vz, wx,wy,wz, position_c, origin_o,
        // e a fun��o dotproduct().
        //
        // ATEN��O: O produto escalar, computado pela fun��o dotproduct(), est�
        // definido somente para argumentos que s�o VETORES. N�o existe produto
        // escalar de PONTOS.
        //
        ux, uy, uz, -dotproduct(u, vc),
        vx, vy, vz, -dotproduct(v, vc),
        wx, wy, wz, -dotproduct(w, vc),
        0.0f,0.0f,0.0f,1.0f
    );
}

// Matriz de proje��o paralela ortogr�fica
glm::mat4 Matrix_Orthographic(float l, float r, float b, float t, float n, float f)
{
    glm::mat4 M = Matrix(
        // PREENCHA AQUI A MATRIZ M DE PROJE��O ORTOGR�FICA (3D) UTILIZANDO OS
        // PAR�METROS l,r,b,t,n,f
        2.0f/(r-l), 0.0f      , 0.0f      , -(r+l)/(r-l),  // LINHA 1
        0.0f      , 2.0f/(t-b), 0.0f      , -(t+b)/(t-b),  // LINHA 2
        0.0f      , 0.0f      , 2.0f/(f-n), -(f+n)/(f-n), // LINHA 3
        0.0f      , 0.0f      , 0.0f      , 1.0f          // LINHA 4
    );
    return M;
}

// Matriz de proje��o perspectiva
glm::mat4 Matrix_Perspective(float field_of_view, float aspect, float n, float f)
{
    float t = fabs(n) * tanf(field_of_view / 2.0f);
    float b = -t;
    float r = t * aspect;
    float l = -r;

    glm::mat4 P = Matrix(
        n, 0.0f, 0.0f, 0.0f,
        0.0f, n, 0.0f, 0.0f,
        0.0f, 0.0f, n+f, -f*n,
        0.0f, 0.0f, 1.0f, 0.0f
    );

    // A matriz M � a mesma computada acima em Matrix_Orthographic().
    glm::mat4 M = Matrix_Orthographic(l, r, b, t, n, f);
    return -M*P;
}

// Fun��o que imprime uma matriz M no terminal
void PrintMatrix(glm::mat4 M)
{
    printf("\n");
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][0], M[1][0], M[2][0], M[3][0]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][1], M[1][1], M[2][1], M[3][1]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][2], M[1][2], M[2][2], M[3][2]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][3], M[1][3], M[2][3], M[3][3]);
}

// Fun��o que imprime um vetor v no terminal
void PrintVector(glm::vec4 v)
{
    printf("\n");
    printf("[ %+0.2f ]\n", v[0]);
    printf("[ %+0.2f ]\n", v[1]);
    printf("[ %+0.2f ]\n", v[2]);
    printf("[ %+0.2f ]\n", v[3]);
}

// Fun��o que imprime o produto de uma matriz por um vetor no terminal
void PrintMatrixVectorProduct(glm::mat4 M, glm::vec4 v)
{
    auto r = M*v;
    printf("\n");
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ]   [ %+0.2f ]\n", M[0][0], M[1][0], M[2][0], M[3][0], v[0], r[0]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ] = [ %+0.2f ]\n", M[0][1], M[1][1], M[2][1], M[3][1], v[1], r[1]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ]   [ %+0.2f ]\n", M[0][2], M[1][2], M[2][2], M[3][2], v[2], r[2]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ]   [ %+0.2f ]\n", M[0][3], M[1][3], M[2][3], M[3][3], v[3], r[3]);
}

// Fun��o que imprime o produto de uma matriz por um vetor, junto com divis�o
// por w, no terminal.
void PrintMatrixVectorProductDivW(glm::mat4 M, glm::vec4 v)
{
    auto r = M*v;
    auto w = r[3];
    printf("\n");
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ]   [ %+0.2f ]            [ %+0.2f ]\n", M[0][0], M[1][0], M[2][0], M[3][0], v[0], r[0], r[0]/w);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ] = [ %+0.2f ] =(div w)=> [ %+0.2f ]\n", M[0][1], M[1][1], M[2][1], M[3][1], v[1], r[1], r[1]/w);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ]   [ %+0.2f ]            [ %+0.2f ]\n", M[0][2], M[1][2], M[2][2], M[3][2], v[2], r[2], r[2]/w);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ]   [ %+0.2f ]            [ %+0.2f ]\n", M[0][3], M[1][3], M[2][3], M[3][3], v[3], r[3], r[3]/w);
}
