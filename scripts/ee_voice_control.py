#!/usr/bin/env python
"""
Uses keyboard keys to update the cartesian pose goal for the end effector. The updates are published to the end_effector_command_pose_stamped topic, which can then be used by inverseKinematics routine, which in turns moves the arm.

Derived from the end_effector_keyboard_control script by BIRL Robotics
"""



import copy

import rospy
import time
import baxter_interface
import baxter_external_devices
import tf

from baxter_interface import CHECK_VERSION

from std_msgs.msg import (
	String,
)

from geometry_msgs.msg import (
    Point,
    Quaternion,
    Pose,
    PoseStamped,
)

commandPoseStampedPublisher = rospy.Publisher("end_effector_command_pose_stamped", PoseStamped, queue_size=1)

# Default variables
current_limb = 'right'
current_rotation = 'x'
limbPoseStamped = PoseStamped()
global_move_speed = 0.01
global_rotate_speed = 0.001
global_speed_factor = 0.001

#
# def map_keyboard():
# 	rospy.loginfo('press ? to print help')
# 	while not rospy.is_shutdown():
# 		c = baxter_external_devices.getch()
# 		if c:
# 			#catch Esc or ctrl-c
# 			if c in ['\x1b', '\x03']:
# 				rospy.signal_shutdown("Finished.Exiting...")
# 				return
# 			checkCommand(c)
#
#
# def checkCommand(command):
# 	global limbPoseStamped
# 	global global_move_speed
# 	global global_rotate_speed
# 	global global_speed_factor
# 	global current_limb
# 	global current_rotation
#
#
# 	if (command == 'w'):
# 		limbPoseStamped.pose.position.z += global_move_speed
# 	elif (command == 's'):
# 		limbPoseStamped.pose.position.z -= global_move_speed
# 	elif (command == 'a'):
# 		limbPoseStamped.pose.position.y += global_move_speed
# 	elif (command == 'd'):
# 		limbPoseStamped.pose.position.y -= global_move_speed
# 	elif (command == 'q'):
# 		limbPoseStamped.pose.position.x -= global_move_speed
# 	elif (command == 'e'):
# 		limbPoseStamped.pose.position.x += global_move_speed
# 	elif (command == 'k'):
# 		global_move_speed += global_speed_factor
# 		# printControlState()
# 	elif (command == 'l'):
# 		global_move_speed -= global_speed_factor
# 		if (global_move_speed < 0):
# 			global_move_speed = 0
# 		# printControlState()
# 	elif (command == 'r'):
# 		rotateLimb(True)
# 	elif (command == 't'):
# 		rotateLimb(False)
# 	elif (command == 'i'):
# 		global_rotate_speed += global_speed_factor
# 		# printControlState()
# 	elif (command == 'o'):
# 		global_rotate_speed -= global_speed_factor
# 		if (global_rotate_speed < 0):
# 			global_rotate_speed = 0
# 		# printControlState()
# 	elif (command == 'f'):
# 		if (current_limb == 'right'):
# 			current_limb = 'left'
# 		else:
# 			current_limb = 'right'
# 		initLimbPose()
# 		# printControlState()
# 	elif (command == 'x'):
# 		current_rotation = 'x'
# 		# printControlState()
# 	elif (command == 'y'):
# 		current_rotation = 'y'
# 		# printControlState()
# 	elif (command == 'z'):
# 		current_rotation = 'z'
# 		# printControlState()
#
# 	## Caution!
# 	commandPoseStampedPublisher.publish(limbPoseStamped)
#
# def rotateLimb(clockwise):
# 	global current_rotation
# 	global global_rotate_speed
# 	global limbPoseStamped
#
# 	quaternion = (
# 		limbPoseStamped.pose.orientation.x,
# 		limbPoseStamped.pose.orientation.y,
# 		limbPoseStamped.pose.orientation.z,
# 		limbPoseStamped.pose.orientation.w,
# 	)
# 	euler = tf.transformations.euler_from_quaternion(quaternion)
#
# 	target_rotate_speed = copy.copy(global_rotate_speed)
# 	if not clockwise:
# 		target_rotate_speed = -target_rotate_speed
#
# 	r = 0
# 	p = 0
# 	y = 0
# 	if (current_rotation == 'x'):
# 		r = target_rotate_speed
# 	elif (current_rotation == 'y'):
# 		p = target_rotate_speed
# 	elif (current_rotation == 'z'):
# 		y = target_rotate_speed
#
# 	quaternion = tf.transformations.quaternion_from_euler(euler[0] + r, euler[1] + p, euler[2] + y)
# 	limbPoseStamped.pose.orientation.x = quaternion[0]
# 	limbPoseStamped.pose.orientation.y = quaternion[1]
# 	limbPoseStamped.pose.orientation.z = quaternion[2]
# 	limbPoseStamped.pose.orientation.w = quaternion[3]
# 	print 'rotated...'

#
# def printControlState():
# 	print("\nControling %s limb ..." % current_limb)
# 	print("Move speed is: %.3lf meter per press" % global_move_speed)
# 	print("Controling rotation %s ..." % current_rotation)
# 	print("Rotate speed is: %.3lf radius per press" % global_rotate_speed)

def initLimbPose():
	global limbPoseStamped
	global current_limb

	limb = baxter_interface.Limb(current_limb)
	currentPose = dict()
	while not ("position" in currentPose):
		currentPose = limb.endpoint_pose()

	limbPoseStamped.pose.position = Point(
				currentPose["position"].x,
				currentPose["position"].y,
				currentPose["position"].z,
			)
	limbPoseStamped.pose.orientation = Quaternion(
				currentPose["orientation"].x,
				currentPose["orientation"].y,
				currentPose["orientation"].z,
				currentPose["orientation"].w,
			)

	limbPoseStamped.header.frame_id = current_limb
	limbPoseStamped.header.stamp = rospy.Time().now()

# On recieving command from Kaldi Relay
def commandCallback(data):
	# Log Command
	print("Command: " + data.data)
	cmd = str(data.data)
	# Parse command in to robot movement
	parseCommand(cmd)


# Robot can take commands
listening = True
# Double-Checking enabled
dc = False
# Requiring Confirmation (extension of DC)
req_conf = False
# Confirmation Pass (extension of DC)
conf_pass = False

# Line of code to execute in repeaterCallback
constructed_cmd = ''
# Backup
saved_constr_cmd = ''

# Turn a string command in to a robot command
def parseCommand(command):
	global listening, dc, req_conf, conf_pass, constructed_cmd, saved_constr_cmd, override

	cmd = command		# Duplicate for modification
	confirmed = False	# Command confirmed (for DC only)
	cancel = False		# Cancel command (for DC only)
	blocked = False		# Block command
	dc_switch = False	# Double Check switch condition
	override = False	# Override in case of Stopping
	ignore = False		# Ignore command

	# Command groups
	pre_moves = ["GO", "MOVE"]
	arms = ["LEFT ARM", "RIGHT ARM"]
	moves = ["LEFT", "RIGHT", "UP", "DOWN", "IN", "OUT"]
	confs = ["YES", "NO"]
	start_L = "START LISTENING"
	stop_L = "STOP LISTENING"
	cease = "STOP "
	dcon = "DOUBLE CHECK MY COMMANDS"
	dcoff = "STOP DOUBLE CHECKING COMMANDS"

	pre_moves_in_cmd = []
	arms_in_cmd = []
	moves_in_cmd = []
	confirmations_in_cmd = []

	# Group words in command into appropriate groups above
	for word in pre_moves:
		if word in cmd:
			pre_moves_in_cmd.append(word)
			cmdTrimmed = cmd.replace(word,'')
			cmd = cmdTrimmed
	for phrase in arms:
		if phrase in cmd:
			arms_in_cmd.append(phrase)
			cmdTrimmed = cmd.replace(phrase,'')
			cmd = cmdTrimmed
	for move in moves:
		if move in cmd:
			moves_in_cmd.append(move)
			cmdTrimmed = cmd.replace(move,'')
			cmd = cmdTrimmed
	for c in confs:
		if c in cmd:
			confirmations_in_cmd.append(c)
			cmdTrimmed = cmd.replace(c,'')
			cmd = cmdTrimmed

	# Emergency Stop
	if command == cease:
		print("STOPPING")
		conf_pass = False
		confirmed = True
		override = True
	elif command == "!SIL ":
		blocked = True

	# Set start and stop listening
	if start_L in command:
		listening = True
	if stop_L in command:
		listening = False

	# Set start and stop double checking commands
	if dcon in command:
		dc = True
		dc_switch = True
	if dcoff in command:
		dc = False
		dc_switch = True

	# Change limb based on phrase
	for x in arms_in_cmd:
		if x != current_limb:
			changeLimb(x.lower())

	#### BUILD COMMAND
	# Command parts
	cmd_pre = "limbPoseStamped.pose.position"
	cmd_axis = ''
	cmd_modifier = '='
	cmd_value = 'global_move_speed'

	if override or dc_switch or blocked:
		cmd_value = cmd_pre
	elif len(moves_in_cmd)>0:
		for m in moves_in_cmd:
			if m==moves[0]:
				cmd_axis = '.y'
				cmd_modifier = '+' + cmd_modifier
			elif m==moves[1]:
				cmd_axis = '.y'
				cmd_modifier = '-' + cmd_modifier
			elif m==moves[2]:
				cmd_axis = '.z'
				cmd_modifier = '+' + cmd_modifier
			elif m==moves[3]:
				cmd_axis = '.z'
				cmd_modifier = '-' + cmd_modifier
			elif m==moves[4]:
				cmd_axis = '.x'
				cmd_modifier = '-' + cmd_modifier
			elif m==moves[5]:
				cmd_axis = '.x'
				cmd_modifier = '+' + cmd_modifier
	else:
		ignore = True

	# If double checking, then require confirmation
	if dc:
		req_conf = True

	# Approve if confirmation given (used in confirmation pass)
	for c in confirmations_in_cmd:
		if c == "YES":
			if req_conf:
				confirmed = True
		elif c == "NO":
			if req_conf:
				cancel = True

	# If double checking, Prompt the double check
	if dc and req_conf and not dc_switch and not conf_pass and not blocked and not cancel:
		print("Do you want me to " + command + "?")
		saved_constr_cmd = cmd_pre + cmd_axis + cmd_modifier + cmd_value
		conf_pass = True

	# Cancel the command
	if cancel:
		print("Cancelling Command")
		conf_pass = False

	# If robot can take commands, do thems
	if listening:
		if dc and confirmed and conf_pass and not cancel:
			print("Command confirmed")
			constructed_cmd = saved_constr_cmd
			conf_pass = False
		elif dc_switch:
			constructed_cmd = cmd_pre + cmd_axis + cmd_modifier + cmd_value
			saved_constr_cmd = constructed_cmd
			conf_pass = False
		elif ignore:
			constructed_cmd = saved_constr_cmd
		elif not blocked and not dc:
			print("Standard Command Published")
			constructed_cmd = cmd_pre + cmd_axis + cmd_modifier + cmd_value
			saved_constr_cmd = constructed_cmd




# Repeatedly call the last valid command
def repeaterCallback(event):
	global limbPoseStamped
	global global_move_speed
	if constructed_cmd != '':
		# Execute string as code, constructed above
		exec(constructed_cmd)
		commandPoseStampedPublisher.publish(limbPoseStamped)

# Change the current limb to be operated on.
def changeLimb(limb):
	global current_limb
	if "right" in limb:
		print("setting right limb")
		current_limb = "right"
	elif "left" in limb:
		print("setting left limb")
		current_limb = "left"
	elif "both" in limb:
		current_limb = "both"
	initLimbPose()



def main():
	# Init node
	rospy.init_node("voice_control")
	print("Running Voice Controller")

	time.sleep(3) # Hold to avoid race-conditions

	# Setup
	try:
		initLimbPose()
	except():
		pass
	rospy.Subscriber("kaldi_relay", String, commandCallback)
	rospy.Timer(rospy.Duration(0.1), repeaterCallback)
	rospy.spin()



if __name__ == '__main__':
	main()
