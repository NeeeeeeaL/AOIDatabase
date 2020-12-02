/*
 Navicat Premium Data Transfer

 Source Server         : NeaL
 Source Server Type    : MySQL
 Source Server Version : 80017
 Source Host           : localhost:3306
 Source Schema         : pcbe_stl

 Target Server Type    : MySQL
 Target Server Version : 80017
 File Encoding         : 65001

 Date: 01/12/2020 13:11:44
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for 10_op_broadside_extrude
-- ----------------------------
DROP TABLE IF EXISTS `10_op_broadside_extrude`;
CREATE TABLE `10_op_broadside_extrude`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `ele_length` mediumint(9) NULL DEFAULT NULL,
  `ele_width` mediumint(9) NULL DEFAULT NULL,
  `threshold` double NULL DEFAULT NULL,
  `upper_boundary` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `lower_boundary` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 11_op_electrode_extrude
-- ----------------------------
DROP TABLE IF EXISTS `11_op_electrode_extrude`;
CREATE TABLE `11_op_electrode_extrude`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `roi_image` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `template_image` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `electrode_direction` mediumint(9) NULL DEFAULT NULL,
  `inspectionArea_width` double NULL DEFAULT NULL,
  `electrode_num` mediumint(9) NULL DEFAULT NULL,
  `electrode_length` double NULL DEFAULT NULL,
  `threshold` double NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 12_op_pad_infiltration
-- ----------------------------
DROP TABLE IF EXISTS `12_op_pad_infiltration`;
CREATE TABLE `12_op_pad_infiltration`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `pad_width` mediumint(9) NULL DEFAULT NULL,
  `ele_depth` mediumint(9) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 13_op_tin_width
-- ----------------------------
DROP TABLE IF EXISTS `13_op_tin_width`;
CREATE TABLE `13_op_tin_width`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `roi_image` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `template_image` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `electrode_direction` mediumint(9) NULL DEFAULT NULL,
  `inspectionArea_width` double NULL DEFAULT NULL,
  `electrode_num` mediumint(9) NULL DEFAULT NULL,
  `electrode_length` double NULL DEFAULT NULL,
  `threshold` double NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 14_op_electrode_exists_or_not
-- ----------------------------
DROP TABLE IF EXISTS `14_op_electrode_exists_or_not`;
CREATE TABLE `14_op_electrode_exists_or_not`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `threshold` double NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 15_op_pad_copper_leakage
-- ----------------------------
DROP TABLE IF EXISTS `15_op_pad_copper_leakage`;
CREATE TABLE `15_op_pad_copper_leakage`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `ele_type` mediumint(9) NULL DEFAULT NULL,
  `ele_length` mediumint(9) NULL DEFAULT NULL,
  `ele_width` mediumint(9) NULL DEFAULT NULL,
  `threshold` double NULL DEFAULT NULL,
  `upper_boundary` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `lower_boundary` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 16_op_pad_abnormal
-- ----------------------------
DROP TABLE IF EXISTS `16_op_pad_abnormal`;
CREATE TABLE `16_op_pad_abnormal`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `ele_type` mediumint(9) NULL DEFAULT NULL,
  `ele_length` mediumint(9) NULL DEFAULT NULL,
  `ele_width` mediumint(9) NULL DEFAULT NULL,
  `threshold` double NULL DEFAULT NULL,
  `upper_boundary` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `lower_boundary` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 17_op_fill_tin_length
-- ----------------------------
DROP TABLE IF EXISTS `17_op_fill_tin_length`;
CREATE TABLE `17_op_fill_tin_length`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `electrode_direction` mediumint(9) NULL DEFAULT NULL,
  `threshold` double NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 18_op_electrode_up_copper_leakage
-- ----------------------------
DROP TABLE IF EXISTS `18_op_electrode_up_copper_leakage`;
CREATE TABLE `18_op_electrode_up_copper_leakage`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `roi_image` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `template_image` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `electrode_direction` mediumint(9) NULL DEFAULT NULL,
  `inspectionArea_width` double NULL DEFAULT NULL,
  `smallest_copper_leakage_area` double NULL DEFAULT NULL,
  `threshold` double NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 19_op_electrode_infiltration
-- ----------------------------
DROP TABLE IF EXISTS `19_op_electrode_infiltration`;
CREATE TABLE `19_op_electrode_infiltration`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `ele_width` mediumint(9) NULL DEFAULT NULL,
  `ele_length` mediumint(9) NULL DEFAULT NULL,
  `ele_depth` mediumint(9) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 1_op_ending_extrude
-- ----------------------------
DROP TABLE IF EXISTS `1_op_ending_extrude`;
CREATE TABLE `1_op_ending_extrude`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `ele_length` mediumint(9) NULL DEFAULT NULL,
  `ele_width` mediumint(9) NULL DEFAULT NULL,
  `threshold` double NULL DEFAULT NULL,
  `upper_boundary` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `lower_boundary` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 20_op_ele_height
-- ----------------------------
DROP TABLE IF EXISTS `20_op_ele_height`;
CREATE TABLE `20_op_ele_height`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `ele_depth` mediumint(9) NULL DEFAULT NULL,
  `ele_width` mediumint(9) NULL DEFAULT NULL,
  `ele_length` mediumint(9) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 21_op_electrode_bridge
-- ----------------------------
DROP TABLE IF EXISTS `21_op_electrode_bridge`;
CREATE TABLE `21_op_electrode_bridge`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `roi_image` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `template_image` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `electrode_direction` mediumint(9) NULL DEFAULT NULL,
  `inspectionArea_width` double NULL DEFAULT NULL,
  `smallest_pad_area` double NULL DEFAULT NULL,
  `electrode_num` mediumint(9) NULL DEFAULT NULL,
  `electrode_max_width` double NULL DEFAULT NULL,
  `bridgr_max_width` double NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 22_op_electrode_uneven
-- ----------------------------
DROP TABLE IF EXISTS `22_op_electrode_uneven`;
CREATE TABLE `22_op_electrode_uneven`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `pin_num` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `threshold` double NULL DEFAULT NULL,
  `ele_upper_boundary` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `ele_lower_boundary` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `pin_upper_boundary` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `pin_lower_boundary` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 23_op_tin_height
-- ----------------------------
DROP TABLE IF EXISTS `23_op_tin_height`;
CREATE TABLE `23_op_tin_height`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `electrode_direction` mediumint(9) NULL DEFAULT NULL,
  `threshold_1` double NULL DEFAULT NULL,
  `threshold_2` double NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 24_op_pad_something
-- ----------------------------
DROP TABLE IF EXISTS `24_op_pad_something`;
CREATE TABLE `24_op_pad_something`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `ele_type` mediumint(9) NULL DEFAULT NULL,
  `ele_length` mediumint(9) NULL DEFAULT NULL,
  `ele_width` mediumint(9) NULL DEFAULT NULL,
  `threshold` double NULL DEFAULT NULL,
  `upper_boundary` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `lower_boundary` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 25_op_tin_ball
-- ----------------------------
DROP TABLE IF EXISTS `25_op_tin_ball`;
CREATE TABLE `25_op_tin_ball`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `nothing` varchar(10) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 26_op_something
-- ----------------------------
DROP TABLE IF EXISTS `26_op_something`;
CREATE TABLE `26_op_something`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `ele_template` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `ele_roi_width` mediumint(9) NULL DEFAULT NULL,
  `ele_roi_height` mediumint(9) NULL DEFAULT NULL,
  `rate_l` double NULL DEFAULT NULL,
  `rate_w` double NULL DEFAULT NULL,
  `area_upper_boundary` mediumint(9) NULL DEFAULT NULL,
  `area_lower_boundary` mediumint(9) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 27_op_electrode_raise
-- ----------------------------
DROP TABLE IF EXISTS `27_op_electrode_raise`;
CREATE TABLE `27_op_electrode_raise`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `ele_template` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `ele_type` mediumint(9) NULL DEFAULT NULL,
  `ele_depth` mediumint(9) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 2_op_wrong_element
-- ----------------------------
DROP TABLE IF EXISTS `2_op_wrong_element`;
CREATE TABLE `2_op_wrong_element`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `electrode_width` mediumint(9) NULL DEFAULT NULL,
  `electrode_height` mediumint(9) NULL DEFAULT NULL,
  `threshold` mediumint(9) NULL DEFAULT NULL,
  `ele_type` mediumint(9) NULL DEFAULT NULL,
  `character_name` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `upper_boundary` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `lower_boundary` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 3_op_backwards
-- ----------------------------
DROP TABLE IF EXISTS `3_op_backwards`;
CREATE TABLE `3_op_backwards`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `threshold` double NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 4_op_ending_overlap
-- ----------------------------
DROP TABLE IF EXISTS `4_op_ending_overlap`;
CREATE TABLE `4_op_ending_overlap`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `roi_image` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `template_image` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `threshold` double NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 5_op_missing_element
-- ----------------------------
DROP TABLE IF EXISTS `5_op_missing_element`;
CREATE TABLE `5_op_missing_element`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `threshold` double NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 6_op_electrode_area
-- ----------------------------
DROP TABLE IF EXISTS `6_op_electrode_area`;
CREATE TABLE `6_op_electrode_area`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `roi_image` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `template_image` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `electrode_direction` mediumint(9) NULL DEFAULT NULL,
  `inspectionArea_width` double NULL DEFAULT NULL,
  `electrode_num` mediumint(9) NULL DEFAULT NULL,
  `electrode_area` double NULL DEFAULT NULL,
  `threshold` double NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 7_op_polarity_error
-- ----------------------------
DROP TABLE IF EXISTS `7_op_polarity_error`;
CREATE TABLE `7_op_polarity_error`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `ele_width` mediumint(9) NULL DEFAULT NULL,
  `ele_height` mediumint(9) NULL DEFAULT NULL,
  `ele_type` mediumint(9) NULL DEFAULT NULL,
  `polarity_distribution` mediumint(9) NULL DEFAULT NULL,
  `upper_boundary` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `lower_boundary` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 8_op_electrode_lateral_bending
-- ----------------------------
DROP TABLE IF EXISTS `8_op_electrode_lateral_bending`;
CREATE TABLE `8_op_electrode_lateral_bending`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `pin_num` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `threshold` double NULL DEFAULT NULL,
  `ele_upper_boundary` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `ele_lower_boundary` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `pin_upper_boundary` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `pin_lower_boundary` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `pin_length` mediumint(9) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for 9_op_element_offset
-- ----------------------------
DROP TABLE IF EXISTS `9_op_element_offset`;
CREATE TABLE `9_op_element_offset`  (
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `x_threshold` mediumint(9) NULL DEFAULT NULL,
  `y_threshold` mediumint(9) NULL DEFAULT NULL,
  `r_threshold` mediumint(9) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for elements_list
-- ----------------------------
DROP TABLE IF EXISTS `elements_list`;
CREATE TABLE `elements_list`  (
  `id` bigint(20) UNSIGNED NOT NULL AUTO_INCREMENT,
  `ele_name` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `ele_index` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `ele_length` mediumint(9) NULL DEFAULT NULL,
  `ele_width` mediumint(9) NULL DEFAULT NULL,
  `ele_height` mediumint(9) NULL DEFAULT NULL,
  `ele_angel` mediumint(9) NULL DEFAULT NULL,
  `ng_trigger` varchar(80) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `pic_path` varchar(100) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

SET FOREIGN_KEY_CHECKS = 1;
