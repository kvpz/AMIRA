#ifndef POSEPREDICTION_H
#define POSEPREDICTION_H


inline rs2_quaternion quaternion_exp(rs2_vector v)
{
    float x = v.x/2, y = v.y/2, z = v.z/2, th2, th = sqrtf(th2 = x*x + y*y + z*z);
    float c = cosf(th), s = th2 < sqrtf(120*FLT_EPSILON) ? 1-th2/6 : sinf(th)/th;
    rs2_quaternion Q = { s*x, s*y, s*z, c };
    return Q;
}

inline rs2_quaternion quaternion_multiply(rs2_quaternion a, rs2_quaternion b)
{
    rs2_quaternion Q = {
        a.x * b.w + a.w * b.x - a.z * b.y + a.y * b.z,
        a.y * b.w + a.z * b.x + a.w * b.y - a.x * b.z,
        a.z * b.w - a.y * b.x + a.x * b.y + a.w * b.z,
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
    };
    return Q;
}


rs2_pose predict_pose(rs2_pose & pose, float dt_s)
{
    rs2_pose P = pose;
    P.translation.x = dt_s * (dt_s/2 * pose.acceleration.x + pose.velocity.x) + pose.translation.x;
    P.translation.y = dt_s * (dt_s/2 * pose.acceleration.y + pose.velocity.y) + pose.translation.y;
    P.translation.z = dt_s * (dt_s/2 * pose.acceleration.z + pose.velocity.z) + pose.translation.z;
    rs2_vector W = {
            dt_s * (dt_s/2 * pose.angular_acceleration.x + pose.angular_velocity.x),
            dt_s * (dt_s/2 * pose.angular_acceleration.y + pose.angular_velocity.y),
            dt_s * (dt_s/2 * pose.angular_acceleration.z + pose.angular_velocity.z),
    };
    P.rotation = quaternion_multiply(quaternion_exp(W), pose.rotation);
    return P;
}

#endif
