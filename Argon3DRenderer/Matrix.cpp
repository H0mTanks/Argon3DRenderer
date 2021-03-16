#include "PrecompiledHeaders.hpp"
#include "Matrix.hpp"


Matrix4::Matrix4()
{
    m.fill({});
}

Matrix4::Matrix4(float a11, float a12, float a13, float a14, float a21, float a22, float a23, float a24, float a31, float a32, float a33, float a34, float a41, float a42, float a43, float a44)
{
    m[0][0] = a11;
    m[0][1] = a12;
    m[0][2] = a13;
    m[0][3] = a14;
    m[1][0] = a21;
    m[1][1] = a22;
    m[1][2] = a23;
    m[1][3] = a24;
    m[2][0] = a31;
    m[2][1] = a32;
    m[2][2] = a33;
    m[2][3] = a34;
    m[3][0] = a41;
    m[3][1] = a42;
    m[3][2] = a43;
    m[3][3] = a44;
}

Matrix4 Matrix4::identity()
{
    return Matrix4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}

Matrix4 Matrix4::make_scale(float a11, float a22, float a33)
{
    Matrix4 id = identity();
    id.m[0][0] = a11;
    id.m[1][1] = a22;
    id.m[2][2] = a33;
    return id;
}

Matrix4 Matrix4::make_translation(float a14, float a24, float a34)
{
    Matrix4 id = identity();
    id.m[0][3] = a14;
    id.m[1][3] = a24;
    id.m[2][3] = a34;
    return id;
}

Matrix4 Matrix4::make_rotation_z(float angle)
{
    float c = cos(angle);
    float s = sin(angle);

    Matrix4 id = identity();
    id.m[0][0] = c;
    id.m[0][1] = -s;
    id.m[1][0] = s;
    id.m[1][1] = c;

    return id;
}

Matrix4 Matrix4::make_rotation_x(float angle)
{
    float c = cos(angle);
    float s = sin(angle);

    Matrix4 id = identity();
    id.m[1][1] = c;
    id.m[1][2] = -s;
    id.m[2][1] = s;
    id.m[2][2] = c;

    return id;
}

Matrix4 Matrix4::make_rotation_y(float angle)
{
    float c = cos(angle);
    float s = sin(angle);

    Matrix4 id = identity();
    id.m[0][0] = c;
    id.m[0][2] = s;
    id.m[2][0] = -s;
    id.m[2][2] = c;

    return id;
}


Matrix4 Matrix4::make_world(Matrix4 const& scale, Matrix4 const& rotation_x, Matrix4 const& rotation_y, Matrix4 const& rotation_z, Matrix4 const& translation)
{
    Matrix4 world_matrix = identity();
    world_matrix = scale.mul_matrix(world_matrix);
    world_matrix = rotation_x.mul_matrix(world_matrix);
    world_matrix = rotation_y.mul_matrix(world_matrix);
    world_matrix = rotation_z.mul_matrix(world_matrix);
    world_matrix = translation.mul_matrix(world_matrix);

    return world_matrix;
}

Matrix4 Matrix4::make_perspective(float fov, float aspect, float znear, float zfar)
{
    Matrix4 m;
    m.m[0][0] = aspect * (1 / tan(fov / 2));
    m.m[1][1] = 1 / tan(fov / 2);
    m.m[2][2] = zfar / (zfar - znear);
    m.m[2][3] = (-zfar * znear) / (zfar - znear);
    m.m[3][2] = 1.0;

    return m;
}

Matrix4 Matrix4::make_third_person(Vector3 const& camera_position, Vector3 const& target, Vector3 const& up)
{
    Vector3 z = target.sub(camera_position);
    z = z.normalize();
    Vector3 x = up.cross(z);
    x = x.normalize();
    Vector3 y = z.cross(x);

    return Matrix4(
        x.x, x.y, x.z, -x.dot(camera_position),
        y.x, y.y, y.z, -y.dot(camera_position),
        z.x, z.y, z.z, -z.dot(camera_position),
        0, 0, 0, 1
    );
}
 
Vector4 Matrix4::mul_project(Vector4 const& v) const
{
    Matrix4 const& projection_matrix = *this;
    Vector4 result = projection_matrix.mul_vector(v);

    if (result.w != 0.0f) {
        result.x /= result.w;
        result.y /= result.w;
        result.z /= result.w;
    }
    return result;
}

Vector4 Matrix4::mul_vector(Vector4 const& v) const
{
    return Vector4(
        m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
        m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
        m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
        m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
    );
}

Matrix4 Matrix4::mul_matrix(Matrix4 const& m2) const
{
    Matrix4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.m[i][j] = m[i][0] * m2.m[0][j] + m[i][1] * m2.m[1][j] + m[i][2] * m2.m[2][j]
                + m[i][3] * m[3][j];
        }
    }
    return result;
}
