// linalg.cpp


#include "linalg.h"
#include <cstdlib>
#include <iostream>
#include <iomanip>

#define FF(x) std::setw(8)<<std::setfill(' ')<<std::fixed<<std::setprecision(4)<<(x)


// ---------------- vec2 ----------------

vec2 operator * ( float k, vec2 const& p )

{
  vec2 q;

  q.x = p.x * k;
  q.y = p.y * k;

  return q;
}


// I/O operators

std::ostream& operator << ( std::ostream& stream, vec2 const& p )

{
  stream << FF(p.x) << " " << FF(p.y);
  return stream;
}

std::istream& operator >> ( std::istream& stream, vec2 & p )

{
  stream >> p.x >> p.y;
  return stream;
}


// ---------------- vec3 ----------------


vec3 operator * ( float k, vec3 const& p )

{
  vec3 q;

  q.x = p.x * k;
  q.y = p.y * k;
  q.z = p.z * k;

  return q;
}


// point-to-line distance

float vec3::distanceToLine( vec3 lineStart, vec3 lineDir )

{
  lineDir = lineDir.normalize();
  vec3 toPoint = *this - lineStart;
  return (toPoint - (toPoint * lineDir)*lineDir).length();
}
vec3 vec3::perp1()

{
  vec3 result(0,0,0);

  if (x == 0)
    if (y == 0 || z == 0)
      result.x = 1;     /* v = (0 0 z) or (0 y 0) */
    else {
      result.y = -z; /* v = (0 y z) */
      result.z = y;
    }
  else if (y == 0)
    if (z == 0)
      result.z = 1;     /* v = (x 0 0) */
    else {
      result.x = -z; /* v = (x 0 z) */
      result.z = x;
    }
  else {
    result.x = -y;   /* v = (x y z) or (x y 0) */
    result.y = x;
  }

  float lenRecip = 1.0 / sqrt( result.x*result.x + result.y*result.y + result.z*result.z );
  result.x *= lenRecip;
  result.y *= lenRecip;
  result.z *= lenRecip;

  return result;
}


vec3 vec3::perp2()

{
  vec3 result(0,0,0);

  if (x == 0)
    if (y == 0)
      result.y = 1;
    else if (z == 0)
      result.z = 1;
    else
      result.x = 1;
  else
    if (y == 0)
      result.y = 1;
    else if (z == 0)
      result.z = 1;
    else {
      result.x = x * z;
      result.y = y * z;
      result.z = - x*x - y*y;

      float lenRecip = 1.0 / sqrt( result.x*result.x + result.y*result.y + result.z*result.z );
      result.x *= lenRecip;
      result.y *= lenRecip;
      result.z *= lenRecip;
    }

  return result;
}


// I/O operators

std::ostream& operator << ( std::ostream& stream, vec3 const& p )

{
  stream << FF(p.x) << " " << FF(p.y) << " " << FF(p.z);
  return stream;
}

std::istream& operator >> ( std::istream& stream, vec3 & p )

{
  stream >> p.x >> p.y >> p.z;
  return stream;
}


// ---------------- vec4 ----------------


vec4 operator * ( float k, vec4 const& p )

{
  vec4 q;

  q.x = p.x * k;
  q.y = p.y * k;
  q.z = p.z * k;
  q.w = p.w * k;

  return q;
}

// I/O operators

std::ostream& operator << ( std::ostream& stream, vec4 const& p )

{
  stream << FF(p.x) << " " << FF(p.y) << " " << FF(p.z) << " " << FF(p.w);
  return stream;
}

std::istream& operator >> ( std::istream& stream, vec4 & p )

{
  stream >> p.x >> p.y >> p.z >> p.w;
  return stream;
}


// ---------------- quaternion ----------------


mat4 quaternion::toMatrix() const

{
  mat4 m;

  m.rows[0] = vec4( 2 * (q0*q0 + q1*q1 - .5),
		    2 * (q1*q2 - q0*q3),
		    2 * (q1*q3 + q0*q2),
		    0 );
  m.rows[1] = vec4( 2 * (q1*q2 + q0*q3),
		    2 * (q0*q0 + q2*q2 - .5),
		    2 * (q2*q3 - q0*q1),
		    0 );
  m.rows[2] = vec4( 2 * (q1*q3 - q0*q2),
		    2 * (q2*q3 + q0*q1),
		    2 * (q0*q0 + q3*q3 - .5),
		    0 );
  m.rows[3] = vec4( 0, 0, 0, 1 );

  return m;
}

quaternion quaternion::derivative( vec3 w ) // w = omega = angular velocity

{
  quaternion q;

  q.q0 = 0.5 * (-q1*w.x - q2*w.y - q3*w.z );
  q.q1 = 0.5 * ( q0*w.x - q3*w.y + q2*w.z );
  q.q2 = 0.5 * ( q3*w.x + q0*w.y - q1*w.z );
  q.q3 = 0.5 * (-q2*w.x + q1*w.y + q0*w.z );

  return q;
}


quaternion operator * ( quaternion const& q1, quaternion const& q2 )

{
  quaternion result;

  result.q0 = q1.q0*q2.q0 - q1.q1*q2.q1 - q1.q2*q2.q2 - q1.q3*q2.q3;
  result.q1 = q1.q0*q2.q1 + q1.q1*q2.q0 + q1.q2*q2.q3 - q1.q3*q2.q2;
  result.q2 = q1.q0*q2.q2 + q1.q2*q2.q0 + q1.q1*q2.q3 - q1.q3*q2.q1;
  result.q3 = q1.q0*q2.q3 + q1.q3*q2.q0 + q1.q1*q2.q2 - q1.q2*q2.q1;

  return result;
}

vec3 operator * ( quaternion const& q, vec3 const& v )

{
  vec4 result = q.toMatrix() * vec4( v, 1 );
  return vec3( result.x, result.y, result.z );
}

quaternion operator * ( float k, quaternion const& q )

{
  float angle = k * q.angle();
  vec3 axis = q.axis();

  return quaternion( cos(angle/2.0), sin(angle/2.0) * axis );
}

// I/O operators

std::ostream& operator << ( std::ostream& stream, quaternion const& q )

{
  stream << FF(q.q0) << " " << FF(q.q1) << " " << FF(q.q2) << " " << FF(q.q3);
  return stream;
}

std::istream& operator >> ( std::istream& stream, quaternion & q )

{
  stream >> q.q0 >> q.q1 >> q.q2 >> q.q3;
  return stream;
}



// ---------------- mat2 ----------------


mat2 operator * ( float k, mat2 const& m )

{
  mat2 out;

  out.rows[0] = k * m.rows[0];
  out.rows[1] = k * m.rows[1];
  
  return out;
}

vec2 operator * ( mat2 const& m, vec2 const& v )

{
  vec2 out;

  out[0] = m.rows[0] * v;
  out[1] = m.rows[1] * v;
  
  return out;
}

mat2 operator * ( mat2 const& m, mat2 const& n )

{
  mat2 out;

  for (int i=0; i<2; i++)
    for (int j=0; j<2; j++) {

      float sum=0;

      for (int k=0; k<2; k++)
	sum += m[i][k] * n[k][j];

      out[i][j] = sum;
    }

  return out;
}

mat2 mat2::inverse()

{
  mat2 inv;

  float detInv = 1.0 / (rows[0][0] * rows[1][1] - rows[0][1] * rows[1][0]);

  inv[0][0] =  detInv * rows[1][1];
  inv[0][1] = -detInv * rows[0][1];
  inv[1][0] = -detInv * rows[1][0];
  inv[1][1] =  detInv * rows[0][0];
  
  return inv;
}


// I/O operators

std::ostream& operator << ( std::ostream& stream, mat2 const& m )

{
  for (int i=0; i<2; i++) {
    for (int j=0; j<2; j++)
      stream << (j>0 ? " " : "") << m[i][j];
    stream << std::endl;
  }

  return stream;
}

std::istream& operator >> ( std::istream& stream, mat2 & m )

{
  for (int i=0; i<2; i++)
    for (int j=0; j<2; j++)
      stream >> m[i][j];

  return stream;
}

// ---------------- mat3 ----------------


mat3 operator * ( float k, mat3 const& m )

{
  mat3 out;

  out.rows[0] = k * m.rows[0];
  out.rows[1] = k * m.rows[1];
  out.rows[2] = k * m.rows[2];

  return out;
}

vec3 operator * ( mat3 const& m, vec3 const& v )

{
  vec3 out;

  out[0] = m.rows[0] * v;
  out[1] = m.rows[1] * v;
  out[2] = m.rows[2] * v;

  return out;
}

mat3 operator * ( mat3 const& m, mat3 const& n )

{
  mat3 out;

  for (int i=0; i<3; i++)
    for (int j=0; j<3; j++) {

      float sum=0;

      for (int k=0; k<3; k++)
	sum += m[i][k] * n[k][j];

      out[i][j] = sum;
    }

  return out;
}

mat3 mat3::inverse()

{
  mat3 inv;

  float det = 
    rows[0][0] * (rows[1][1] * rows[2][2] - rows[2][1] * rows[1][2]) -
    rows[0][1] * (rows[1][0] * rows[2][2] - rows[1][2] * rows[2][0]) +
    rows[0][2] * (rows[1][0] * rows[2][1] - rows[1][1] * rows[2][0]);

  if (det == 0) {
    std::cerr << "Matrix has no inverse" << std::endl;
    exit(1);
  }

  float invdet = 1.0 / det;

  inv[0][0] = (rows[1][1] * rows[2][2] - rows[2][1] * rows[1][2]) * invdet;
  inv[0][1] = (rows[0][2] * rows[2][1] - rows[0][1] * rows[2][2]) * invdet;
  inv[0][2] = (rows[0][1] * rows[1][2] - rows[0][2] * rows[1][1]) * invdet;
  inv[1][0] = (rows[1][2] * rows[2][0] - rows[1][0] * rows[2][2]) * invdet;
  inv[1][1] = (rows[0][0] * rows[2][2] - rows[0][2] * rows[2][0]) * invdet;
  inv[1][2] = (rows[1][0] * rows[0][2] - rows[0][0] * rows[1][2]) * invdet;
  inv[2][0] = (rows[1][0] * rows[2][1] - rows[2][0] * rows[1][1]) * invdet;
  inv[2][1] = (rows[2][0] * rows[0][1] - rows[0][0] * rows[2][1]) * invdet;
  inv[2][2] = (rows[0][0] * rows[1][1] - rows[1][0] * rows[0][1]) * invdet;

  return inv;
}
// I/O operators

std::ostream& operator << ( std::ostream& stream, mat3 const& m )

{
  for (int i=0; i<3; i++) {
    for (int j=0; j<3; j++)
      stream << (j>0 ? " " : "") << FF(m[i][j]);
    stream << std::endl;
  }

  return stream;
}

std::istream& operator >> ( std::istream& stream, mat3 & m )

{
  for (int i=0; i<3; i++)
    for (int j=0; j<3; j++)
      stream >> m[i][j];

  return stream;
}


// ---------------- mat4 ----------------


mat4 operator * ( float k, mat4 const& m )

{
  mat4 out;

  out.rows[0] = k * m.rows[0];
  out.rows[1] = k * m.rows[1];
  out.rows[2] = k * m.rows[2];
  out.rows[3] = k * m.rows[3];

  return out;
}

vec4 operator * ( mat4 const& m, vec4 const& v )

{
  vec4 out;

  out[0] = m.rows[0] * v;
  out[1] = m.rows[1] * v;
  out[2] = m.rows[2] * v;
  out[3] = m.rows[3] * v;

  return out;
}

mat4 operator * ( mat4 const& m, mat4 const& n )

{
  mat4 out;

  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++) {

      float sum=0;

      for (int k=0; k<4; k++)
	sum += m[i][k] * n[k][j];

      out[i][j] = sum;
    }

  return out;
}


// Matrix inverse adapted from Mesa GLU implementation

mat4 mat4::inverse()

{
  mat4 inv;

  inv.rows[0][0] =
    rows[1][1] * rows[2][2] * rows[3][3] - 
    rows[1][1] * rows[2][3] * rows[3][2] - 
    rows[2][1] * rows[1][2] * rows[3][3] + 
    rows[2][1] * rows[1][3] * rows[3][2] +
    rows[3][1] * rows[1][2] * rows[2][3] - 
    rows[3][1] * rows[1][3] * rows[2][2];

  inv.rows[1][0] =
    -rows[1][0] * rows[2][2] * rows[3][3] + 
    rows[1][0] * rows[2][3] * rows[3][2] + 
    rows[2][0] * rows[1][2] * rows[3][3] - 
    rows[2][0] * rows[1][3] * rows[3][2] - 
    rows[3][0] * rows[1][2] * rows[2][3] + 
    rows[3][0] * rows[1][3] * rows[2][2];

  inv.rows[2][0] =
    rows[1][0] * rows[2][1] * rows[3][3] - 
    rows[1][0] * rows[2][3] * rows[3][1] - 
    rows[2][0] * rows[1][1] * rows[3][3] + 
    rows[2][0] * rows[1][3] * rows[3][1] + 
    rows[3][0] * rows[1][1] * rows[2][3] - 
    rows[3][0] * rows[1][3] * rows[2][1];

  inv.rows[3][0] =
    -rows[1][0] * rows[2][1] * rows[3][2] + 
    rows[1][0] * rows[2][2] * rows[3][1] +
    rows[2][0] * rows[1][1] * rows[3][2] - 
    rows[2][0] * rows[1][2] * rows[3][1] - 
    rows[3][0] * rows[1][1] * rows[2][2] + 
    rows[3][0] * rows[1][2] * rows[2][1];

  inv.rows[0][1] =
    -rows[0][1] * rows[2][2] * rows[3][3] + 
    rows[0][1] * rows[2][3] * rows[3][2] + 
    rows[2][1] * rows[0][2] * rows[3][3] - 
    rows[2][1] * rows[0][3] * rows[3][2] - 
    rows[3][1] * rows[0][2] * rows[2][3] + 
    rows[3][1] * rows[0][3] * rows[2][2];

  inv.rows[1][1] =
    rows[0][0] * rows[2][2] * rows[3][3] - 
    rows[0][0] * rows[2][3] * rows[3][2] - 
    rows[2][0] * rows[0][2] * rows[3][3] + 
    rows[2][0] * rows[0][3] * rows[3][2] + 
    rows[3][0] * rows[0][2] * rows[2][3] - 
    rows[3][0] * rows[0][3] * rows[2][2];

  inv.rows[2][1] =
    -rows[0][0] * rows[2][1] * rows[3][3] + 
    rows[0][0] * rows[2][3] * rows[3][1] + 
    rows[2][0] * rows[0][1] * rows[3][3] - 
    rows[2][0] * rows[0][3] * rows[3][1] - 
    rows[3][0] * rows[0][1] * rows[2][3] + 
    rows[3][0] * rows[0][3] * rows[2][1];

  inv.rows[3][1] =
    rows[0][0] * rows[2][1] * rows[3][2] - 
    rows[0][0] * rows[2][2] * rows[3][1] - 
    rows[2][0] * rows[0][1] * rows[3][2] + 
    rows[2][0] * rows[0][2] * rows[3][1] + 
    rows[3][0] * rows[0][1] * rows[2][2] - 
    rows[3][0] * rows[0][2] * rows[2][1];

  inv.rows[0][2] =
    rows[0][1] * rows[1][2] * rows[3][3] - 
    rows[0][1] * rows[1][3] * rows[3][2] - 
    rows[1][1] * rows[0][2] * rows[3][3] + 
    rows[1][1] * rows[0][3] * rows[3][2] + 
    rows[3][1] * rows[0][2] * rows[1][3] - 
    rows[3][1] * rows[0][3] * rows[1][2];

  inv.rows[1][2] =
    -rows[0][0] * rows[1][2] * rows[3][3] + 
    rows[0][0] * rows[1][3] * rows[3][2] + 
    rows[1][0] * rows[0][2] * rows[3][3] - 
    rows[1][0] * rows[0][3] * rows[3][2] - 
    rows[3][0] * rows[0][2] * rows[1][3] + 
    rows[3][0] * rows[0][3] * rows[1][2];

  inv.rows[2][2] =
    rows[0][0] * rows[1][1] * rows[3][3] - 
    rows[0][0] * rows[1][3] * rows[3][1] - 
    rows[1][0] * rows[0][1] * rows[3][3] + 
    rows[1][0] * rows[0][3] * rows[3][1] + 
    rows[3][0] * rows[0][1] * rows[1][3] - 
    rows[3][0] * rows[0][3] * rows[1][1];

  inv.rows[3][2] =
    -rows[0][0] * rows[1][1] * rows[3][2] + 
    rows[0][0] * rows[1][2] * rows[3][1] + 
    rows[1][0] * rows[0][1] * rows[3][2] - 
    rows[1][0] * rows[0][2] * rows[3][1] - 
    rows[3][0] * rows[0][1] * rows[1][2] + 
    rows[3][0] * rows[0][2] * rows[1][1];

  inv.rows[0][3] =
    -rows[0][1] * rows[1][2] * rows[2][3] + 
    rows[0][1] * rows[1][3] * rows[2][2] + 
    rows[1][1] * rows[0][2] * rows[2][3] - 
    rows[1][1] * rows[0][3] * rows[2][2] - 
    rows[2][1] * rows[0][2] * rows[1][3] + 
    rows[2][1] * rows[0][3] * rows[1][2];

  inv.rows[1][3] =
    rows[0][0] * rows[1][2] * rows[2][3] - 
    rows[0][0] * rows[1][3] * rows[2][2] - 
    rows[1][0] * rows[0][2] * rows[2][3] + 
    rows[1][0] * rows[0][3] * rows[2][2] + 
    rows[2][0] * rows[0][2] * rows[1][3] - 
    rows[2][0] * rows[0][3] * rows[1][2];

  inv.rows[2][3] =
    -rows[0][0] * rows[1][1] * rows[2][3] + 
    rows[0][0] * rows[1][3] * rows[2][1] + 
    rows[1][0] * rows[0][1] * rows[2][3] - 
    rows[1][0] * rows[0][3] * rows[2][1] - 
    rows[2][0] * rows[0][1] * rows[1][3] + 
    rows[2][0] * rows[0][3] * rows[1][1];

  inv.rows[3][3] =
    rows[0][0] * rows[1][1] * rows[2][2] - 
    rows[0][0] * rows[1][2] * rows[2][1] - 
    rows[1][0] * rows[0][1] * rows[2][2] + 
    rows[1][0] * rows[0][2] * rows[2][1] + 
    rows[2][0] * rows[0][1] * rows[1][2] - 
    rows[2][0] * rows[0][2] * rows[1][1];

  float det = rows[0][0] * inv.rows[0][0] + rows[0][1] * inv.rows[1][0] + rows[0][2] * inv.rows[2][0] + rows[0][3] * inv.rows[3][0];

  if (det == 0) {
    std::cerr << "Matrix has no inverse" << std::endl;
    exit(1);
  }

  det = 1.0 / det;

  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      inv.rows[i][j] *= det;

  return inv;
}

mat4 scale( float x, float y, float z )

{
  mat4 out;

  out.rows[0] = vec4( x, 0, 0, 0 );
  out.rows[1] = vec4( 0, y, 0, 0 );
  out.rows[2] = vec4( 0, 0, z, 0 );
  out.rows[3] = vec4( 0, 0, 0, 1 );

  return out;
}

mat4 translate( float x, float y, float z )

{
  mat4 out;

  out.rows[0] = vec4( 1, 0, 0, x );
  out.rows[1] = vec4( 0, 1, 0, y );
  out.rows[2] = vec4( 0, 0, 1, z );
  out.rows[3] = vec4( 0, 0, 0, 1 );

  return out;
}


mat4 translate( vec3 v )

{
  mat4 out;

  out.rows[0] = vec4( 1, 0, 0, v.x );
  out.rows[1] = vec4( 0, 1, 0, v.y );
  out.rows[2] = vec4( 0, 0, 1, v.z );
  out.rows[3] = vec4( 0, 0, 0,   1 );

  return out;
}


mat4 rotate( float theta, vec3 axis )

{
  axis = axis.normalize();

  float v1 = axis.x;
  float v2 = axis.y;
  float v3 = axis.z;
  
  float t1 =  cos(theta);
  float t2 =  1 - t1;
  float t3 =  v1*v1;
  float t6 =  t2*v1;
  float t7 =  t6*v2;
  float t8 =  sin(theta);
  float t9 =  t8*v3;
  float t11 = t6*v3;
  float t12 = t8*v2;
  float t15 = v2*v2;
  float t19 = t2*v2*v3;
  float t20 = t8*v1;
  float t24 = v3*v3;

  mat4 out;

  out.rows[0] = vec4( t1 + t2*t3,     t7 - t9,   t11 + t12, 0 );
  out.rows[1] = vec4(    t7 + t9, t1 + t2*t15,   t19 - t20, 0 );
  out.rows[2] = vec4(  t11 - t12,   t19 + t20, t1 + t2*t24, 0 );
  out.rows[3] = vec4(          0,           0,           0, 1 );

  return out;
}


mat4 rotate( vec3 fromVec, vec3 toVec )

{
  fromVec = fromVec.normalize();
  toVec   = toVec.normalize();

  vec3 axis = fromVec ^ toVec;

  float angle = atan2( axis.length(), fromVec*toVec );

  return rotate( angle, axis );
}



mat4 transpose( mat4 M )

{
  mat4 out;

  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      out[i][j] = M[j][i];
      
  return out;
}
mat4 identity4()

{
  mat4 M;

  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      if (i==j)
	M[i][j] = 1;
      else
	M[i][j] = 0;

  return M;
}


mat4 frustum( float l, float r, float b, float t, float n, float f )

{
  mat4 out;

  out.rows[0] = vec4( 2*n/(r-l),         0, (r+l)/(r-l),           0 );
  out.rows[1] = vec4(         0, 2*n/(t-b), (t+b)/(t-b),           0 );
  out.rows[2] = vec4(         0,         0, (f+n)/(n-f), 2*f*n/(n-f) );
  out.rows[3] = vec4(         0,         0,          -1,           0 );

  return out;
}


mat4 perspective( float fovy, float aspect, float n, float f )

{
  mat4 out;

  float s = 1.0 / tan( fovy / 2.0 );

  out.rows[0] = vec4( s/aspect,          0,           0,           0 );
  out.rows[1] = vec4(         0,         s,           0,           0 );
  out.rows[2] = vec4(         0,         0, (f+n)/(n-f), 2*f*n/(n-f) );
  out.rows[3] = vec4(         0,         0,          -1,           0 );

  return out;
}
    

mat4 ortho( float l, float r, float b, float t, float n, float f )

{
  mat4 out;

  out.rows[0] = vec4( 2/(r-l),       0,       0, (l+r)/(l-r) );
  out.rows[1] = vec4(       0, 2/(t-b),       0, (b+t)/(b-t) );
  out.rows[2] = vec4(       0,       0, 2/(n-f), (n+f)/(n-f) );
  out.rows[3] = vec4(       0,       0,       0,           1 );

  return out;
}
    

mat4 lookat( vec3 from, vec3 to, vec3 up )

{
  mat4 M;

  vec3 z = (from - to).normalize();
  vec3 x = (up ^ z).normalize();
  vec3 y = (z ^ x).normalize();

  M.rows[0] = vec4( x.x, x.y, x.z, 0 );
  M.rows[1] = vec4( y.x, y.y, y.z, 0 );
  M.rows[2] = vec4( z.x, z.y, z.z, 0 );
  M.rows[3] = vec4(   0,   0,   0, 1 );

  mat4 ret = M * translate( -1 * from );

  return ret;
}


// I/O operators

std::ostream& operator << ( std::ostream& stream, mat4 const& m )

{
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++)
      stream << (j>0 ? " " : "") << FF(m[i][j]);
    stream << std::endl;
  }

  return stream;
}

std::istream& operator >> ( std::istream& stream, mat4 & m )

{
  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      stream >> m[i][j];

  return stream;
}


// Compute the point-to-edge distance from 'point' to the edge defined
// by 'edgeTail' and 'edgeHead'.  Also set *closestPoint to the
// closest point on the edge, which might be inside the edge or might
// be one of the endpoints.

float pointToEdgeDistance( vec3 point, vec3 edgeTail, vec3 edgeHead, vec3 *closestPoint )

{
  // [YOUR CODE HERE: REPLACE THE CODE BELOW]
  
  *closestPoint = vec3(0,0,0);
  float distAlongPoint = ((point - edgeHead) * (edgeTail - edgeHead)) /
      ((edgeTail - edgeHead) * (edgeTail - edgeHead));

  //vec3 closestPoint = vec3(0, 0, 0);
  if (distAlongPoint <= 0) {
      *closestPoint = edgeHead;
  }
  else if (distAlongPoint >= 1) {
      *closestPoint = edgeTail;
  }
  else {
      *closestPoint = (distAlongPoint * (edgeTail - edgeHead)) + edgeHead;
  }

  
  return (point - *closestPoint).length();
}

