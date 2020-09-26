use aoi;
-- 创建主表`production_result`
create table if not exists `production_result` (`id` bigint unsigned not null auto_increment primary key, `board` varchar(50), 
	`ele_index` varchar(30), `timestamp` datetime, `ele_name` varchar(30), `is_ng` tinyint(1), `ng_type` varchar(80),
	`ng_triger` varchar(80),  `pic_path` varchar(100), `loc_x` mediumint, `loc_y` mediumint, `width` mediumint, `height` mediumint,
	index `search_index`(`timestamp`, `board`, `ele_index`));
-- alter table `production_result` add index search_index(`timestamp`, `board`, `ele_index`);
-- 算子分表00-末端突出
create table if not exists `op_ending_extrude` (`id` bigint unsigned not null auto_increment primary key, `board` varchar(50),  
	`ele_index` varchar(30), `timestamp` datetime, `is_ng` tinyint(1), `percent` varchar(10),
	index `search_index`(`timestamp`, `board`, `ele_index`));
-- 算子分表01-错料
create table if not exists `op_wrong_element` (`id` bigint unsigned not null auto_increment primary key, `board` varchar(50),  
	`ele_index` varchar(30), `timestamp` datetime, `is_ng` tinyint(1), `character` varchar(30),
	index `search_index`(`timestamp`, `board`, `ele_index`));
-- 算子分表02-装反
create table if not exists `op_backwards` (`id` bigint unsigned not null auto_increment primary key, `board` varchar(50),  
	`ele_index` varchar(30), `timestamp` datetime, `is_ng` tinyint(1), `similarity` varchar(10),
	index `search_index`(`timestamp`, `board`, `ele_index`));
-- 算子分表03-末端重叠
create table if not exists `op_ending_overlap` (`id` bigint unsigned not null auto_increment primary key, `board` varchar(50),  
	`ele_index` varchar(30), `timestamp` datetime, `is_ng` tinyint(1), `percent` varchar(10),
	index `search_index`(`timestamp`, `board`, `ele_index`));
-- 算子分表04-缺件
create table if not exists `op_missing_element` (`id` bigint unsigned not null auto_increment primary key, `board` varchar(50),  
	`ele_index` varchar(30), `timestamp` datetime, `is_ng` tinyint(1), `similarity` varchar(10),
	index `search_index`(`timestamp`, `board`, `ele_index`));
-- 算子分表05-电极面积
create table if not exists `op_electrode_area` (`id` bigint unsigned not null auto_increment primary key, `board` varchar(50),  
	`ele_index` varchar(30), `timestamp` datetime, `is_ng` tinyint(1), `percent` varchar(10),
	index `search_index`(`timestamp`, `board`, `ele_index`));
-- 算子分表06-极性错误
create table if not exists `op_polarity_error` (`id` bigint unsigned not null auto_increment primary key, `board` varchar(50),  
	`ele_index` varchar(30), `timestamp` datetime, `is_ng` tinyint(1), `description` varchar(10),
	index `search_index`(`timestamp`, `board`, `ele_index`));
-- 算子分表07-电极横向弯曲
create table if not exists `op_electrode_lateral_bending` (`id` bigint unsigned not null auto_increment primary key, `board` varchar(50),  
	`ele_index` varchar(30), `timestamp` datetime, `is_ng` tinyint(1), `percent` varchar(10),
	index `search_index`(`timestamp`, `board`, `ele_index`));
-- 算子分表08-元件偏移
create table if not exists `op_element_offset` (`id` bigint unsigned not null auto_increment primary key, `board` varchar(50),  
	`ele_index` varchar(30), `timestamp` datetime, `is_ng` tinyint(1), `x_offset` varchar(10), `y_offset` varchar(10), `rotation_angle` varchar(10),
	index `search_index`(`timestamp`, `board`, `ele_index`));
-- 算子分表09-侧边突出
create table if not exists `op_broadside_extrude` (`id` bigint unsigned not null auto_increment primary key, `board` varchar(50),  
	`ele_index` varchar(30), `timestamp` datetime, `is_ng` tinyint(1), `percent1` varchar(10), `percent2` varchar(10),
	index `search_index`(`timestamp`, `board`, `ele_index`));
-- 算子分表10-电极突出
create table if not exists `op_electrode_extrude` (`id` bigint unsigned not null auto_increment primary key, `board` varchar(50),  
	`ele_index` varchar(30), `timestamp` datetime, `is_ng` tinyint(1), `percent` varchar(10),
	index `search_index`(`timestamp`, `board`, `ele_index`));
-- 算子分表11-焊盘浸润
create table if not exists `op_pad_infiltration` (`id` bigint unsigned not null auto_increment primary key, `board` varchar(50),  
	`ele_index` varchar(30), `timestamp` datetime, `is_ng` tinyint(1), `percent` varchar(10),
	index `search_index`(`timestamp`, `board`, `ele_index`));
