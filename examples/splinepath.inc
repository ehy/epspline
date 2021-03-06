// POV-Ray include file
// Generated by epspline


#ifndef ( INCLUDED_splinepath_inc )
#declare INCLUDED_splinepath_inc = 1;

#declare BzPath_1_width = 454.975710;
#declare BzPath_1_height = 441.487038;
#declare BzPath_1_left = 703.619813;
#declare BzPath_1_right = 1158.595523;
#declare BzPath_1_top = 101.109176;
#declare BzPath_1_bottom = 542.596214;
#declare BzPath_1_max_extent = max(BzPath_1_width, BzPath_1_height);

/*
  Undefined type "BzPath_1" was defined as a bezier_spline
  with sweep (min, max) -1.000000 and 1.000000.
  It is declared as an array here.
*/
#declare BzPath_1_ARRAY_COUNT = 64;
#declare BzPath_1_SWEEP_MIN = -1.000000;
#declare BzPath_1_SWEEP_MAX = 1.000000;
#declare BzPath_1 =
array[BzPath_1_ARRAY_COUNT] {
  <846.179650, 234.286590>,
  <857.768428, 216.019126>,
  <816.699713, 116.088093>,
  <836.833164, 107.158138>,
  <836.833164, 107.158138>,
  <857.857895, 97.832866>,
  <916.125499, 193.749258>,
  <933.124155, 193.535479>,
  <933.124155, 193.535479>,
  <948.354497, 193.343939>,
  <999.465624, 96.042803>,
  <1020.209164, 101.306409>,
  <1020.209164, 101.306409>,
  <1037.466738, 105.685463>,
  <1004.690360, 214.302722>,
  <1018.999127, 224.365484>,
  <1018.999127, 224.365484>,
  <1033.284495, 234.411789>,
  <1147.358830, 198.063644>,
  <1157.598220, 218.682928>,
  <1157.598220, 218.682928>,
  <1167.383175, 238.387106>,
  <1065.089923, 297.244154>,
  <1062.485139, 319.942504>,
  <1062.485139, 319.942504>,
  <1060.236543, 339.536988>,
  <1166.199761, 390.993733>,
  <1158.164232, 409.873094>,
  <1158.164232, 409.873094>,
  <1150.974903, 426.764321>,
  <1034.098736, 389.500782>,
  <1022.823623, 403.170890>,
  <1022.823623, 403.170890>,
  <1012.864087, 415.245973>,
  <1058.024731, 531.674728>,
  <1037.000000, 541.000000>,
  <1037.000000, 541.000000>,
  <1016.889388, 549.919825>,
  <956.423103, 441.225802>,
  <938.000000, 441.000000>,
  <938.000000, 441.000000>,
  <920.196843, 440.781796>,
  <863.665668, 551.708837>,
  <844.000000, 542.000000>,
  <844.000000, 542.000000>,
  <822.990180, 531.627562>,
  <867.862017, 407.674659>,
  <850.000000, 397.000000>,
  <850.000000, 397.000000>,
  <829.741180, 384.892969>,
  <713.623826, 438.575487>,
  <704.298554, 417.550756>,
  <704.298554, 417.550756>,
  <694.033466, 394.407118>,
  <800.562298, 339.422722>,
  <802.000000, 318.000000>,
  <802.000000, 318.000000>,
  <803.470083, 296.094776>,
  <698.000000, 231.000000>,
  <709.000000, 212.000000>,
  <709.000000, 212.000000>,
  <721.405075, 190.573052>,
  <832.522144, 255.815002>,
  <846.179650, 234.286590>
}

#declare Path_1_width = 377.000000;
#declare Path_1_height = 408.000000;
#declare Path_1_left = 303.000000;
#declare Path_1_right = 680.000000;
#declare Path_1_top = 38.000000;
#declare Path_1_bottom = 446.000000;
#declare Path_1_max_extent = max(Path_1_width, Path_1_height);

/*
  Undefined type "Path_1" was defined as a cubic_spline
  with sweep (min, max) -1.000000 and 1.000000.
  It is declared as an array here.
*/
#declare Path_1_ARRAY_COUNT = 19;
#declare Path_1_SWEEP_MIN = -1.000000;
#declare Path_1_SWEEP_MAX = 1.000000;
#declare Path_1 =
array[Path_1_ARRAY_COUNT] {
  <489.000000, 37.000000>,
  <534.999818, 125.479183>,
  <621.999455, 101.885291>,
  <593.000363, 196.285239>,
  <680.000000, 243.472717>,
  <593.000363, 290.684882>,
  <621.999455, 385.084829>,
  <534.999818, 361.490938>,
  <490.000000, 446.000000>,
  <448.000182, 361.490938>,
  <361.000583, 385.084829>,
  <389.999599, 290.684882>,
  <303.000000, 243.472717>,
  <389.999599, 196.285239>,
  <361.000583, 101.885291>,
  <448.000182, 125.479183>,
  <489.000000, 38.000000>,
  <534.999818, 125.479183>,
  <621.999455, 101.885291>
}

#declare Handle_width = 132.705067;
#declare Handle_height = 189.654925;
#declare Handle_left = 149.294933;
#declare Handle_right = 282.000000;
#declare Handle_top = 114.735475;
#declare Handle_bottom = 304.390400;
#declare Handle_max_extent = max(Handle_width, Handle_height);

#declare Handle_NORMAL_TRANSFORM = transform {
  translate <-Handle_left - Handle_width / 2, 0, -Handle_top - Handle_height / 2>
  scale <1 / Handle_max_extent, 1, 1 / Handle_max_extent>
  scale <1, 1, -1>
}

#declare Handle =
prism { bezier_spline linear_sweep -20.000000, 20.000000
  , 32
  , <159.000000, 158.155556>
  , <164.656380, 136.837287>
  , <168.000000, 137.000000>
  , <173.933333, 147.488889>
  , <173.933333, 147.488889>
  , <190.000000, 106.000000>
  , <281.000000, 92.000000>
  , <282.000000, 178.000000>
  , <282.000000, 178.000000>
  , <282.574326, 227.392017>
  , <219.200000, 259.111111>
  , <168.000000, 284.000000>
  , <168.000000, 284.000000>
  , <152.000000, 324.000000>
  , <147.288889, 295.688889>
  , <150.000000, 280.000000>
  , <150.000000, 280.000000>
  , <153.157604, 261.727308>
  , <155.022222, 266.888889>
  , <160.000000, 274.000000>
  , <160.000000, 274.000000>
  , <200.533333, 250.533333>
  , <269.972649, 225.001132>
  , <268.000000, 177.000000>
  , <268.000000, 177.000000>
  , <265.000000, 104.000000>
  , <198.800000, 124.977778>
  , <177.000000, 160.000000>
  , <177.000000, 160.000000>
  , <165.800000, 182.977778>
  , <154.000000, 177.000000>
  , <159.000000, 158.155556>
#ifdef ( Handle_USE_NORMAL_TRANSFORM )
  transform { Handle_NORMAL_TRANSFORM }
#end
}

#declare Cup_width = 370.000000;
#declare Cup_height = 303.515625;
#declare Cup_left = -185.000000;
#declare Cup_right = 185.000000;
#declare Cup_top = 85.750000;
#declare Cup_bottom = 389.265625;
#declare Cup_max_extent = max(Cup_width, Cup_height);

#declare Cup_NORMAL_TRANSFORM = transform {
  translate <0, -Cup_top - Cup_height / 2, 0>
  scale (1 / Cup_max_extent)
  scale <1, -1, 1>
}

#declare Cup =
lathe { bezier_spline
  32
  , <0.000000, 378.000000>
  , <33.333333, 378.000000>
  , <70.000000, 378.000000>
  , <92.222222, 379.000000>
  , <92.222222, 379.000000>
  , <92.000000, 392.000000>
  , <95.000000, 389.000000>
  , <102.000000, 389.000000>
  , <102.000000, 389.000000>
  , <108.000000, 389.000000>
  , <117.000000, 391.000000>
  , <108.888889, 376.000000>
  , <108.888889, 376.000000>
  , <181.000000, 377.000000>
  , <139.000000, 177.000000>
  , <185.000000, 88.000000>
  , <185.000000, 88.000000>
  , <178.333333, 84.000000>
  , <181.111111, 86.000000>
  , <170.000000, 88.000000>
  , <170.000000, 88.000000>
  , <127.000000, 174.000000>
  , <164.000000, 355.000000>
  , <105.000000, 363.000000>
  , <105.000000, 363.000000>
  , <70.555556, 367.000000>
  , <38.888889, 366.000000>
  , <0.000000, 366.000000>
  , <0.000000, 366.000000>
  , <0.000000, 373.000000>
  , <0.000000, 373.000000>
  , <0.000000, 378.000000>
#ifdef ( Cup_USE_NORMAL_TRANSFORM )
  transform { Cup_NORMAL_TRANSFORM }
#end
}

#declare I_splinepath_inc_width = 1343.595523;
#declare I_splinepath_inc_height = 504.596214;
#declare I_splinepath_inc_left = -185.000000;
#declare I_splinepath_inc_right = 1158.595523;
#declare I_splinepath_inc_top = 38.000000;
#declare I_splinepath_inc_bottom = 542.596214;

#end // #ifdef ( INCLUDED_splinepath_inc )

