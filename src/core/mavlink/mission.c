#include <stdint.h>
#include "mavlink.h"
#include "../mavlink/publisher.h"
#include "autopilot.h"

static void mavlink_send_capability(void)
{
	mavlink_message_t msg;

	uint64_t cap = MAV_PROTOCOL_CAPABILITY_MAVLINK2 |
	               MAV_PROTOCOL_CAPABILITY_SET_ATTITUDE_TARGET;
	uint32_t flight_sw_version = 1;
	uint32_t middleware_sw_ver = 2;
	uint32_t os_sw_version = 10; //freertos ver. = 10.2.1
	uint32_t board_version = 1;
	uint8_t *flight_custom_ver = NULL;
	uint8_t *middleware_custom_ver = NULL;
	uint8_t *os_custom_ver = NULL;
	uint16_t vendor_id = 10000;
	uint16_t product_id = 10001;
	uint64_t uid = 100;

	mavlink_msg_autopilot_version_pack_chan(1, 1, MAVLINK_COMM_1, &msg, cap, flight_sw_version, middleware_sw_ver,
	                                        os_sw_version, board_version, flight_custom_ver, middleware_custom_ver,
	                                        os_custom_ver, vendor_id, product_id, uid, NULL);
	send_mavlink_msg_to_uart(&msg);
}

static void mav_cmd_long_takeoff(void)
{
	autopilot_trigger_auto_takeoff();
}

static void mav_cmd_long_land(void)
{
	autopilot_trigger_auto_landing();
}

static void mav_cmd_long_override_goto(mavlink_command_long_t *cmd_long)
{
}

static void mav_cmd_long_guided_enable(mavlink_command_long_t *cmd_long)
{
	if(cmd_long->param1 > 0.5f) {
		autopilot_trajectory_following_start();
	} else {
		autopilot_trajectory_following_halt();
	}
}

void mav_command_long(mavlink_message_t *received_msg)
{
	mavlink_command_long_t mav_command_long;
	mavlink_msg_command_long_decode(received_msg, &mav_command_long);

	switch(mav_command_long.command) {
	case MAV_CMD_REQUEST_AUTOPILOT_CAPABILITIES:
		mavlink_send_capability();
		break;
	case MAV_CMD_COMPONENT_ARM_DISARM:
		break;
	case MAV_CMD_NAV_TAKEOFF:
		mav_cmd_long_takeoff();
		break;
	case MAV_CMD_NAV_LAND:
		mav_cmd_long_land();
		break;
	case MAV_CMD_OVERRIDE_GOTO:
		mav_cmd_long_override_goto(&mav_command_long);
		break;
	case MAV_CMD_NAV_GUIDED_ENABLE:
		mav_cmd_long_guided_enable(&mav_command_long);
	}
}

void mav_mission_request_list(mavlink_message_t *received_msg)
{
	mavlink_message_t msg;

	uint16_t waypoint_cnt = 0;
	mavlink_msg_mission_count_pack_chan(1, 1, MAVLINK_COMM_1, &msg, 255, 0,
	                                    waypoint_cnt, MAV_MISSION_TYPE_MISSION);
	send_mavlink_msg_to_uart(&msg);
}

void mav_position_target_local_ned(mavlink_message_t *received_msg)
{
	float pos[3];
	float vel[3];
	float yaw;

	mavlink_set_position_target_local_ned_t new_target;
	mavlink_msg_set_position_target_local_ned_decode(received_msg, &new_target);

	pos[0] = new_target.x;
	pos[1] = new_target.y;
	pos[2] = new_target.z;
	vel[0] = new_target.vx;
	vel[1] = new_target.vy;
	vel[2] = new_target.vz;
	yaw = new_target.yaw;

	autopilot_set_trajectory_following_waypoint(pos, vel, yaw);
}

void mav_polynomial_trajectory_write(mavlink_message_t *received_msg)
{
}

void mav_polynomial_trajectory_cmd(mavlink_message_t *received_msg)
{
}

void mav_polynomial_trajectory_item(mavlink_message_t *received_msg)
{
}

void mission_waypoint_ack_handler(void)
{
}

void polynomial_trajectory_ack_handler(void)
{
}
