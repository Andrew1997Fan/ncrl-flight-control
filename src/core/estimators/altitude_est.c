#include "debug_link.h"
#include "optitrack.h"
#include "barometer.h"

float alt_rate_last = 0.0f;
float alt_rate_predict = 0.0f;
float alt_rate_fused = 0.0f;

/* altitude rate estimation using barometer and acceleromter with complementary filter algorithm */
float barometer_alt_rate_estimate(float *dcm, float alt_rate, float *accel_body, float dt)
{
	/* convert measured acceleration from body-fixed frame to earth frame and
	 * take the z-direction component out only */
	float accel_earth_z = dcm[2*3 + 0] * accel_body[0] +
	                      dcm[2*3 + 1] * accel_body[1] +
	                      dcm[2*3 + 2] * accel_body[2];

	/* gravity cancellation */
	accel_earth_z -= -9.81;
	accel_earth_z *= -1; //FIXME

	/* calculate altitude rate by doing numerical integration */
	alt_rate_predict = alt_rate_last + (accel_earth_z * dt);

	/* complementary filter */
	const float a = 0.995;
	alt_rate_fused = (a * alt_rate_predict) + ((1.0f - a) * alt_rate);

	/* save fused altitude rate for next iteration */
	alt_rate_last = alt_rate_fused;

	return alt_rate_fused;
}

void send_alt_est_debug_message(debug_msg_t *payload)
{
	float altitude = 0.0f;
	float optitrack_pos[3] = {0.0f};
	altitude = barometer_get_relative_altitude();
	optitrack_read_pos(optitrack_pos);

	float optitrack_vel[3] = {0.0f};
	optitrack_read_vel(optitrack_vel);

	pack_debug_debug_message_header(payload, MESSAGE_ID_ALT_EST);
	/* position */
	pack_debug_debug_message_float(&altitude, payload);
	pack_debug_debug_message_float(&optitrack_pos[2], payload);
	/* velocity */
	pack_debug_debug_message_float(&alt_rate_fused, payload);
	pack_debug_debug_message_float(&optitrack_vel[2], payload);
}