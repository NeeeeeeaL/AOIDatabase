use pcbe_stl;
-- 创建主表`elements_list`
create table if not exists `elements_list` (`id` bigint unsigned not null auto_increment primary key, `ele_name` varchar(30), 
	`ele_index` varchar(30), `ele_length` mediumint, `ele_width` mediumint, `ele_height` mediumint, `ele_angel` mediumint, 
	`ng_trigger` varchar(80), `pic_path` varchar(100));

-- 算子分表01-末端突出(5个参数)
create table if not exists `1_op_ending_extrude` (`ele_length` mediumint,  `ele_width` mediumint, `threshold` double, 
	`upper_boundary` varchar(40), `lower_boundary` varchar(40));
	
-- 算子分表02-错料(7个参数)
create table if not exists `2_op_wrong_element` (`electrode_width` mediumint, `electrode_height` mediumint, `threshold` mediumint, 
	`ele_type` mediumint, `character_name` varchar(40), `upper_boundary` varchar(40), `lower_boundary` varchar(40));
	
-- 算子分表03-装反(1个参数)
create table if not exists `3_op_backwards` (`threshold` double);

-- 算子分表04-末端重叠(3个参数)
create table if not exists `4_op_ending_overlap` (`roi_image` varchar(40), `template_image` varchar(40),`threshold` double);
	
-- 算子分表05-缺件(1个参数)
create table if not exists `5_op_missing_element` (`threshold` double);

-- 算子分表06-电极面积(7个参数)
create table if not exists `6_op_electrode_area` (`roi_image` varchar(40), `template_image` varchar(40), `electrode_direction` mediumint,  
	`inspectionArea_width` double, `electrode_num` mediumint, `electrode_area` double, `threshold` double);
	
-- 算子分表07-极性错误(6个参数)
create table if not exists `7_op_polarity_error` (`ele_width` mediumint, `ele_height` mediumint, `ele_type` mediumint, 
	`polarity_distribution` mediumint, `upper_boundary` varchar(40), `lower_boundary` varchar(40));
	
-- 算子分表08-电极横向弯曲(7个参数)
create table if not exists `8_op_electrode_lateral_bending` (`pin_num` varchar(40), `threshold` double, `ele_upper_boundary` varchar(40), 
	`ele_lower_boundary` varchar(40), `pin_upper_boundary` varchar(40), `pin_lower_boundary` varchar(40), `pin_length` mediumint);
	
-- 算子分表09-元件偏移(3个参数)
create table if not exists `9_op_element_offset` (`x_threshold` mediumint, `y_threshold` mediumint, `r_threshold` mediumint);
	
-- 算子分表10-侧边突出(5个参数)
create table if not exists `10_op_broadside_extrude` (`ele_length` mediumint, `ele_width` mediumint, `threshold` double,
	`upper_boundary` varchar(40), `lower_boundary` varchar(40));
	
-- 算子分表11-电极突出(7个参数)
create table if not exists `11_op_electrode_extrude` (`roi_image` varchar(40), `template_image` varchar(40), `electrode_direction` mediumint, 
	`inspectionArea_width` double, `electrode_num` mediumint, `electrode_length` double, `threshold` double);
	
-- 算子分表12-焊盘浸润(2个参数)
create table if not exists `12_op_pad_infiltration` (`pad_width` mediumint, `ele_depth` mediumint);
	
-- 算子分表13-填锡末端接合宽度(7个参数)
create table if not exists `13_op_tin_width` (`roi_image` varchar(40), `template_image` varchar(40), `electrode_direction` mediumint,
	`inspectionArea_width` double, `electrode_num` mediumint, `electrode_length` double, `threshold` double);

-- 算子分表14-电极有无(1个参数)
create table if not exists `14_op_electrode_exists_or_not` (`threshold` double); 

-- 算子分表15-焊盘漏铜(6个参数)
create table if not exists `15_op_pad_copper_leakage` (`ele_type` mediumint, `ele_length` mediumint, `ele_width` mediumint, 
	`threshold` double, `upper_boundary` varchar(40), `lower_boundary` varchar(40));

-- 算子分表16-焊盘异常(6个参数)
create table if not exists `16_op_pad_abnormal` (`ele_type` mediumint, `ele_length` mediumint, `ele_width` mediumint, `threshold` double,
	`upper_boundary` varchar(40), `lower_boundary` varchar(40));

-- 算子分表17-填锡长度(2个参数)
create table if not exists `17_op_fill_tin_length` (`electrode_direction` mediumint, `threshold` double);

-- 算子分表18-电极顶端漏铜(6个参数)
create table if not exists  `18_op_electrode_up_copper_leakage` (`roi_image` varchar(40), `template_image` varchar(40), 
	`electrode_direction` mediumint, `inspectionArea_width` double, `smallest_copper_leakage_area` double, `threshold` double);

-- 算子分表19-电极浸润(3个参数)
create table if not exists `19_op_electrode_infiltration` (`ele_width` mediumint, `ele_length` mediumint, `ele_depth` mediumint);

-- 算子分表20-元件高度(3个参数)
create table if not exists `20_op_ele_height` (`ele_depth` mediumint, `ele_width` mediumint, `ele_length` mediumint);

-- 算子分表21-电极桥接(8个参数)
create table if not exists `21_op_electrode_bridge` (`roi_image` varchar(40), `template_image` varchar(40), 
	`electrode_direction` mediumint, `inspectionArea_width` double, `smallest_pad_area` double, `electrode_num` mediumint,
	`electrode_max_width` double, `bridgr_max_width` double);

-- 算子分表22-电极不平(6个参数)
create table if not exists `22_op_electrode_uneven` (`pin_num` varchar(40), `threshold` double, `ele_upper_boundary` varchar(40),
	`ele_lower_boundary` varchar(40), `pin_upper_boundary` varchar(40), `pin_lower_boundary` varchar(40));

-- 算子分表23-填锡高度(3个参数)
create table if not exists `23_op_tin_height` (`electrode_direction` mediumint, `threshold_1` double,`threshold_2` double);

-- 算子分表24-焊盘异物(6个参数)
create table if not exists `24_op_pad_something` (`ele_type` mediumint, `ele_length` mediumint, `ele_width` mediumint, `threshold` double,
	`upper_boundary` varchar(40), `lower_boundary` varchar(40));
	
-- 算子分表25-锡珠(0个参数)	
create table if not exists `25_op_tin_ball`(`nothing` varchar(10));

-- 算子分表26-异物(7个参数)	
create table if not exists `26_op_something`(`ele_template` varchar(40), `ele_roi_width` mediumint, `ele_roi_height` mediumint,
	`rate_l` double, `rate_w` double, `area_upper_boundary` mediumint, `area_lower_boundary` mediumint
);

-- 算子分表27-元件起翘(3个参数)
create table if not exists `27_op_electrode_raise` (`ele_template` varchar(40), `ele_type` mediumint, `ele_depth` mediumint);










