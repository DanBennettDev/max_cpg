{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 7,
			"minor" : 3,
			"revision" : 4,
			"architecture" : "x64",
			"modernui" : 1
		}
,
		"rect" : [ 100.0, 100.0, 1286.0, 626.0 ],
		"bglocked" : 0,
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 1,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 1,
		"objectsnaponopen" : 1,
		"statusbarvisible" : 2,
		"toolbarvisible" : 1,
		"lefttoolbarpinned" : 0,
		"toptoolbarpinned" : 0,
		"righttoolbarpinned" : 0,
		"bottomtoolbarpinned" : 0,
		"toolbars_unpinned_last_save" : 0,
		"tallnewobj" : 0,
		"boxanimatetime" : 200,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"description" : "",
		"digest" : "",
		"tags" : "",
		"style" : "",
		"subpatcher_template" : "",
		"showontab" : 1,
		"title" : "CPG Network Basics",
		"boxes" : [ 			{
				"box" : 				{
					"hidden" : 1,
					"id" : "obj-12",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 3,
							"revision" : 4,
							"architecture" : "x64",
							"modernui" : 1
						}
,
						"rect" : [ 0.0, 26.0, 1286.0, 600.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 1,
						"gridsize" : [ 15.0, 15.0 ],
						"gridsnaponopen" : 1,
						"objectsnaponopen" : 1,
						"statusbarvisible" : 2,
						"toolbarvisible" : 1,
						"lefttoolbarpinned" : 0,
						"toptoolbarpinned" : 0,
						"righttoolbarpinned" : 0,
						"bottomtoolbarpinned" : 0,
						"toolbars_unpinned_last_save" : 0,
						"tallnewobj" : 0,
						"boxanimatetime" : 200,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"description" : "",
						"digest" : "",
						"tags" : "",
						"style" : "",
						"subpatcher_template" : "",
						"showontab" : 1,
						"title" : "External Signals and Sync",
						"boxes" : [ 							{
								"box" : 								{
									"format" : 6,
									"id" : "obj-36",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 724.5, 145.0, 50.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-34",
									"linecount" : 6,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 805.75, 141.0, 231.0, 89.0 ],
									"style" : "",
									"text" : "Nodes will entrain to input signals, in the same way they entrain to one-another\n\nInputs to node 0 have no effect when sync'd to a phasor, other nodes will still be affected"
								}

							}
, 							{
								"box" : 								{
									"format" : 6,
									"id" : "obj-29",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 986.75, 256.0, 50.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-30",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 969.75, 285.0, 36.0, 22.0 ],
									"style" : "",
									"text" : "*~ 0."
								}

							}
, 							{
								"box" : 								{
									"format" : 6,
									"id" : "obj-31",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 905.5, 256.0, 50.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-32",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 888.5, 285.0, 36.0, 22.0 ],
									"style" : "",
									"text" : "*~ 0."
								}

							}
, 							{
								"box" : 								{
									"format" : 6,
									"id" : "obj-26",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 822.75, 256.0, 50.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-27",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 805.75, 285.0, 36.0, 22.0 ],
									"style" : "",
									"text" : "*~ 0."
								}

							}
, 							{
								"box" : 								{
									"format" : 6,
									"id" : "obj-25",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 741.5, 256.0, 50.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-22",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 724.5, 285.0, 36.0, 22.0 ],
									"style" : "",
									"text" : "*~ 0."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-21",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 724.5, 182.0, 65.0, 22.0 ],
									"style" : "",
									"text" : "cycle~ 1.6"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-18",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 953.833313, 79.5, 94.0, 34.0 ],
									"style" : "",
									"text" : "synchronising phasor"
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-13",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 4,
									"outlettype" : [ "", "", "", "" ],
									"patching_rect" : [ 998.0, 500.5, 58.0, 22.0 ],
									"restore" : 									{
										"freq0" : [ 2.79 ],
										"freq1" : [ 10.0 ],
										"freq2" : [ 10.0 ],
										"freq3" : [ 10.0 ]
									}
,
									"style" : "",
									"text" : "autopattr",
									"varname" : "u838007623"
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-107",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 998.0, 420.5, 72.0, 22.0 ],
									"style" : "",
									"text" : "loadmess 1"
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-104",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 998.0, 467.5, 192.0, 22.0 ],
									"presentation" : 1,
									"presentation_linecount" : 2,
									"presentation_rect" : [ 989.0, 562.5, 187.0, 36.0 ],
									"saved_object_attributes" : 									{
										"client_rect" : [ 1057, 45, 1440, 407 ],
										"parameter_enable" : 0,
										"storage_rect" : [ 583, 69, 1034, 197 ]
									}
,
									"style" : "",
									"text" : "pattrstorage others @savemode 2",
									"varname" : "others"
								}

							}
, 							{
								"box" : 								{
									"format" : 6,
									"id" : "obj-12",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 1052.0, 24.5, 50.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-2",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 1052.0, 79.5, 64.0, 22.0 ],
									"style" : "",
									"text" : "phasor~ 1"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-20",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 708.666687, 85.5, 152.333313, 34.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 546.666687, 101.5, 325.333313, 20.0 ],
									"style" : "",
									"text" : "Frequencies for nodes 1-3 (as multiple of root node)"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-19",
									"linecount" : 3,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 395.5, 24.5, 465.5, 48.0 ],
									"presentation" : 1,
									"presentation_linecount" : 3,
									"presentation_rect" : [ 403.5, 58.5, 429.0, 48.0 ],
									"style" : "",
									"text" : "Frequency for node 0 \n(still set this if you sync to external phasor) Changing this independently will change frequencies of other nodes."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-8",
									"maxclass" : "scope~",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 827.0, 383.0, 130.0, 130.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 822.0, 214.0, 130.0, 130.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-9",
									"maxclass" : "scope~",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 676.333313, 383.0, 130.0, 130.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 684.333313, 214.0, 130.0, 130.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-7",
									"maxclass" : "scope~",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 538.666687, 383.0, 130.0, 130.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 546.666687, 214.0, 130.0, 130.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-24",
									"maxclass" : "scope~",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 401.0, 383.0, 130.0, 130.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 409.0, 214.0, 130.0, 130.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"format" : 6,
									"id" : "obj-5",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 644.75, 94.5, 50.0, 22.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 822.0, 128.5, 50.0, 22.0 ],
									"style" : "",
									"varname" : "freq3"
								}

							}
, 							{
								"box" : 								{
									"format" : 6,
									"id" : "obj-6",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 563.5, 94.5, 50.0, 22.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 684.333313, 128.5, 50.0, 22.0 ],
									"style" : "",
									"varname" : "freq2"
								}

							}
, 							{
								"box" : 								{
									"format" : 6,
									"id" : "obj-4",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 482.25, 94.5, 50.0, 22.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 546.666687, 128.5, 50.0, 22.0 ],
									"style" : "",
									"varname" : "freq1"
								}

							}
, 							{
								"box" : 								{
									"format" : 6,
									"id" : "obj-3",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 401.0, 94.5, 50.0, 22.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 409.0, 128.5, 50.0, 22.0 ],
									"style" : "",
									"varname" : "freq0"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-1",
									"maxclass" : "newobj",
									"numinlets" : 9,
									"numoutlets" : 4,
									"outlettype" : [ "signal", "signal", "signal", "signal" ],
									"patching_rect" : [ 401.0, 339.5, 669.0, 22.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 409.0, 169.5, 432.0, 22.0 ],
									"style" : "",
									"text" : "cpg.network~ 4 1000 0 1 1 7 1 4.07 4.07"
								}

							}
, 							{
								"box" : 								{
									"angle" : 270.0,
									"bgcolor" : [ 0.952941, 0.564706, 0.098039, 0.45 ],
									"border" : 1,
									"bordercolor" : [ 0.870588, 0.415686, 0.062745, 1.0 ],
									"id" : "obj-57",
									"maxclass" : "panel",
									"mode" : 0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 468.333313, 79.5, 403.0, 46.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 541.333313, 94.5, 338.0, 65.0 ],
									"proportion" : 0.39,
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"angle" : 270.0,
									"bgcolor" : [ 0.960784, 0.827451, 0.156863, 0.45 ],
									"border" : 1,
									"bordercolor" : [ 0.764706, 0.592157, 0.101961, 1.0 ],
									"id" : "obj-56",
									"maxclass" : "panel",
									"mode" : 0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 391.5, 24.5, 479.833313, 101.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 399.5, 58.5, 479.833313, 101.0 ],
									"proportion" : 0.39,
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontface" : 1,
									"fontsize" : 16.0,
									"id" : "obj-28",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 13.0, 8.5, 257.0, 43.0 ],
									"presentation" : 1,
									"presentation_linecount" : 2,
									"presentation_rect" : [ 22.0, 49.5, 257.0, 43.0 ],
									"style" : "",
									"text" : "Central Pattern Generator network - other features"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-16",
									"linecount" : 16,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 13.0, 65.0, 351.0, 227.0 ],
									"presentation" : 1,
									"presentation_linecount" : 16,
									"presentation_rect" : [ 22.0, 106.0, 363.0, 227.0 ],
									"style" : "",
									"text" : "External signal inlets are enabled y setting argument 4 to 1 this creates a set of signal inlets - simply send your signal into the relevant inlet\n\nExternal sync allows synchronisation to other sequencers, etc.\nIt is enabled by setting argument 5  to '1' or '2'  - this creates another inlet, which will always be the last inlet. \n1 drives node 0 (and the quantiser) directly from a phasor - in this mode, feedback signals to node 0 have no effect\n2 resets node0 to the beginning of its cycle when the incoming phasor wraps. Feedback signals still have some effect on node 0, within this externally imposed cycle. \nIf you use external sync, you should still send a frequency value into the frequency inlet for node 0 as this is used to tell the object the expected root frequency - used for setting frequencies of other nodes and other purposes."
								}

							}
, 							{
								"box" : 								{
									"angle" : 270.0,
									"bgcolor" : [ 1.0, 1.0, 1.0, 0.45 ],
									"border" : 1,
									"bordercolor" : [ 0.239216, 0.254902, 0.278431, 1.0 ],
									"id" : "obj-10",
									"maxclass" : "panel",
									"mode" : 0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 11.0, 8.5, 350.0, 298.5 ],
									"presentation" : 1,
									"presentation_rect" : [ 16.5, 43.5, 359.0, 517.0 ],
									"proportion" : 0.39,
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"angle" : 270.0,
									"bgcolor" : [ 0.317647, 0.654902, 0.976471, 0.45 ],
									"border" : 1,
									"bordercolor" : [ 0.011765, 0.396078, 0.752941, 1.0 ],
									"id" : "obj-65",
									"maxclass" : "panel",
									"mode" : 0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 715.0, 135.5, 332.833313, 197.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 423.0, 367.5, 232.833313, 217.0 ],
									"proportion" : 0.39,
									"style" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-24", 0 ],
									"source" : [ "obj-1", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-7", 0 ],
									"source" : [ "obj-1", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-8", 0 ],
									"source" : [ "obj-1", 3 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-9", 0 ],
									"source" : [ "obj-1", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-104", 0 ],
									"hidden" : 1,
									"source" : [ "obj-107", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-2", 0 ],
									"order" : 0,
									"source" : [ "obj-12", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-3", 0 ],
									"midpoints" : [ 1061.5, 75.0, 410.5, 75.0 ],
									"order" : 1,
									"source" : [ "obj-12", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 8 ],
									"source" : [ "obj-2", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-22", 0 ],
									"order" : 3,
									"source" : [ "obj-21", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-27", 0 ],
									"midpoints" : [ 734.0, 237.0, 815.25, 237.0 ],
									"order" : 2,
									"source" : [ "obj-21", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-30", 0 ],
									"midpoints" : [ 734.0, 237.0, 979.25, 237.0 ],
									"order" : 0,
									"source" : [ "obj-21", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-32", 0 ],
									"midpoints" : [ 734.0, 237.0, 898.0, 237.0 ],
									"order" : 1,
									"source" : [ "obj-21", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 4 ],
									"source" : [ "obj-22", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-22", 1 ],
									"source" : [ "obj-25", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-27", 1 ],
									"source" : [ "obj-26", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 5 ],
									"source" : [ "obj-27", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-30", 1 ],
									"source" : [ "obj-29", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"source" : [ "obj-3", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 7 ],
									"source" : [ "obj-30", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-32", 1 ],
									"source" : [ "obj-31", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 6 ],
									"source" : [ "obj-32", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-21", 0 ],
									"source" : [ "obj-36", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 1 ],
									"source" : [ "obj-4", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 3 ],
									"source" : [ "obj-5", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 2 ],
									"source" : [ "obj-6", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 1157.0, 37.0, 54.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "",
					"text" : "p others",
					"varname" : "others"
				}

			}
, 			{
				"box" : 				{
					"hidden" : 1,
					"id" : "obj-11",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 3,
							"revision" : 4,
							"architecture" : "x64",
							"modernui" : 1
						}
,
						"rect" : [ 0.0, 26.0, 1286.0, 600.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 1,
						"gridsize" : [ 15.0, 15.0 ],
						"gridsnaponopen" : 1,
						"objectsnaponopen" : 1,
						"statusbarvisible" : 2,
						"toolbarvisible" : 1,
						"lefttoolbarpinned" : 0,
						"toptoolbarpinned" : 0,
						"righttoolbarpinned" : 0,
						"bottomtoolbarpinned" : 0,
						"toolbars_unpinned_last_save" : 0,
						"tallnewobj" : 0,
						"boxanimatetime" : 200,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"description" : "",
						"digest" : "",
						"tags" : "",
						"style" : "",
						"subpatcher_template" : "",
						"showontab" : 1,
						"title" : "Note Triggers and Quantiser",
						"boxes" : [ 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-2",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 4,
									"outlettype" : [ "", "", "", "" ],
									"patching_rect" : [ 1197.0, 275.5, 58.0, 22.0 ],
									"restore" : 									{
										"freq0" : [ 2.0 ],
										"freq1" : [ 10.0 ],
										"freq2" : [ 10.0 ],
										"freq3" : [ 10.0 ],
										"weight" : [ 0.0 ],
										"weightfrom" : [ 0 ]
									}
,
									"style" : "",
									"text" : "autopattr",
									"varname" : "u123007621"
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-107",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 1197.0, 195.5, 72.0, 22.0 ],
									"style" : "",
									"text" : "loadmess 1"
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-104",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 1197.0, 242.5, 199.0, 22.0 ],
									"saved_object_attributes" : 									{
										"client_rect" : [ 1057, 45, 1440, 407 ],
										"parameter_enable" : 0,
										"storage_rect" : [ 583, 69, 1034, 197 ]
									}
,
									"style" : "",
									"text" : "pattrstorage triggers @savemode 2",
									"varname" : "triggers"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-54",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 385.0, 472.25, 739.0, 34.0 ],
									"style" : "",
									"text" : "try setting nodes to high values - e.g. 10, and then setting different quantiser and offset values to see the effect on the wave output, and the triggers generated"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-64",
									"linecount" : 5,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 376.5, 182.5, 209.0, 75.0 ],
									"presentation" : 1,
									"presentation_linecount" : 5,
									"presentation_rect" : [ 447.5, 386.0, 218.0, 75.0 ],
									"style" : "",
									"text" : "Output offset is applied to the signal, before quantisation, and can also be used to alter rhythmic output\nValue: 0-1, multiple of node 0 cycle length"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-61",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 379.5, 267.5, 36.0, 20.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 520.166687, 412.0, 36.0, 20.0 ],
									"style" : "",
									"text" : "node"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-59",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 498.5, 267.5, 70.0, 20.0 ],
									"presentation" : 1,
									"presentation_linecount" : 2,
									"presentation_rect" : [ 447.5, 412.0, 44.0, 34.0 ],
									"style" : "",
									"text" : "offset (0-1)"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-44",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 379.5, 399.5, 113.0, 22.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 447.5, 573.0, 113.0, 22.0 ],
									"style" : "",
									"text" : "offset_out 0 0."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-47",
									"maxclass" : "number",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 379.5, 289.5, 50.0, 22.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 520.166687, 434.0, 50.0, 22.0 ],
									"style" : "",
									"varname" : "weightfrom"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-40",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 379.5, 322.5, 99.0, 22.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 520.166687, 500.0, 98.0, 22.0 ],
									"style" : "",
									"text" : "set offset_out $1"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-48",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 379.5, 362.5, 130.0, 22.0 ],
									"presentation" : 1,
									"presentation_linecount" : 2,
									"presentation_rect" : [ 447.5, 536.0, 113.0, 36.0 ],
									"style" : "",
									"text" : "prepend offset_out 0 1"
								}

							}
, 							{
								"box" : 								{
									"format" : 6,
									"id" : "obj-30",
									"maxclass" : "flonum",
									"maximum" : 1.0,
									"minimum" : 0.0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 498.5, 289.5, 50.0, 22.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 447.5, 434.0, 50.0, 22.0 ],
									"style" : "",
									"varname" : "weight"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-43",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 530.5, 43.0, 47.0, 20.0 ],
									"style" : "",
									"text" : "value"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-42",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 489.5, 43.0, 36.0, 20.0 ],
									"style" : "",
									"text" : "node"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-41",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 375.5, 43.0, 45.0, 20.0 ],
									"style" : "",
									"text" : "name"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-39",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 375.5, 17.5, 150.0, 20.0 ],
									"style" : "",
									"text" : "Quantiser Variables"
								}

							}
, 							{
								"box" : 								{
									"bgmode" : 0,
									"border" : 0,
									"clickthrough" : 0,
									"embed" : 1,
									"enablehscroll" : 0,
									"enablevscroll" : 0,
									"id" : "obj-37",
									"lockeddragscroll" : 0,
									"maxclass" : "bpatcher",
									"name" : "quantselecta.maxpat",
									"numinlets" : 0,
									"numoutlets" : 1,
									"offset" : [ 0.0, 0.0 ],
									"outlettype" : [ "" ],
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 7,
											"minor" : 3,
											"revision" : 4,
											"architecture" : "x64",
											"modernui" : 1
										}
,
										"rect" : [ 67.0, 110.0, 640.0, 480.0 ],
										"bglocked" : 0,
										"openinpresentation" : 1,
										"default_fontsize" : 12.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 1,
										"gridsize" : [ 15.0, 15.0 ],
										"gridsnaponopen" : 1,
										"objectsnaponopen" : 1,
										"statusbarvisible" : 2,
										"toolbarvisible" : 1,
										"lefttoolbarpinned" : 0,
										"toptoolbarpinned" : 0,
										"righttoolbarpinned" : 0,
										"bottomtoolbarpinned" : 0,
										"toolbars_unpinned_last_save" : 0,
										"tallnewobj" : 0,
										"boxanimatetime" : 200,
										"enablehscroll" : 1,
										"enablevscroll" : 1,
										"devicewidth" : 0.0,
										"description" : "",
										"digest" : "",
										"tags" : "",
										"style" : "",
										"subpatcher_template" : "",
										"boxes" : [ 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 12.0,
													"id" : "obj-41",
													"linecount" : 2,
													"maxclass" : "message",
													"numinlets" : 2,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 502.25, 220.0, 725.0, 36.0 ],
													"style" : "",
													"text" : "script sendbox gridType presentation_rect -100 -100, script sendbox gridMultiplier presentation_rect -100 -100, script sendbox amount presentation_rect -100 -100, script sendbox offset presentation_rect 159 5"
												}

											}
, 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 12.0,
													"id" : "obj-42",
													"linecount" : 2,
													"maxclass" : "message",
													"numinlets" : 2,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 428.5, 263.0, 846.0, 36.0 ],
													"style" : "",
													"text" : "script sendbox gridType presentation_rect -100 11, script sendbox gridMultiplier presentation_rect -100 -100, script sendbox amount presentation_rect 159 5, script sendbox offset presentation_rect -100 -100"
												}

											}
, 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 12.0,
													"id" : "obj-40",
													"linecount" : 2,
													"maxclass" : "message",
													"numinlets" : 2,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 354.75, 318.0, 890.0, 36.0 ],
													"style" : "",
													"text" : "script sendbox gridType presentation_rect -100 -100, script sendbox gridMultiplier presentation_rect 159 5, script sendbox amount presentation_rect -100 -100, script sendbox offset presentation_rect -100 -100"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-39",
													"maxclass" : "newobj",
													"numinlets" : 1,
													"numoutlets" : 2,
													"outlettype" : [ "", "" ],
													"patching_rect" : [ 151.5, 75.0, 33.0, 22.0 ],
													"style" : "",
													"text" : "t s s"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-38",
													"maxclass" : "newobj",
													"numinlets" : 5,
													"numoutlets" : 5,
													"outlettype" : [ "bang", "bang", "bang", "bang", "" ],
													"patching_rect" : [ 281.0, 186.0, 347.0, 22.0 ],
													"style" : "",
													"text" : "select quant_grid quant_mult quant_amount_node quant_offset"
												}

											}
, 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 12.0,
													"id" : "obj-37",
													"linecount" : 2,
													"maxclass" : "message",
													"numinlets" : 2,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 281.0, 360.0, 971.0, 36.0 ],
													"style" : "",
													"text" : "script sendbox gridType presentation_rect 159 5, script sendbox gridMultiplier presentation_rect -100 -100, script sendbox amount presentation_rect -100 -100, script sendbox offset presentation_rect -100 -100"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-35",
													"maxclass" : "newobj",
													"numinlets" : 1,
													"numoutlets" : 2,
													"outlettype" : [ "", "" ],
													"patching_rect" : [ 281.0, 412.0, 69.0, 22.0 ],
													"save" : [ "#N", "thispatcher", ";", "#Q", "end", ";" ],
													"style" : "",
													"text" : "thispatcher"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-34",
													"items" : [ 1, ",", 2, ",", 3 ],
													"maxclass" : "umenu",
													"numinlets" : 1,
													"numoutlets" : 3,
													"outlettype" : [ "int", "", "" ],
													"parameter_enable" : 0,
													"patching_rect" : [ 66.0, 30.0, 33.0, 22.0 ],
													"presentation" : 1,
													"presentation_rect" : [ 124.0, 5.0, 33.0, 22.0 ],
													"style" : "",
													"varname" : "nodeID"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-33",
													"maxclass" : "message",
													"numinlets" : 2,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 66.0, 325.0, 169.0, 22.0 ],
													"style" : "",
													"text" : "quant_amount_node 1 0."
												}

											}
, 											{
												"box" : 												{
													"comment" : "",
													"id" : "obj-30",
													"index" : 1,
													"maxclass" : "outlet",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 152.0, 284.0, 30.0, 30.0 ],
													"style" : ""
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-28",
													"maxclass" : "message",
													"numinlets" : 2,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 50.0, 75.0, 43.0, 22.0 ],
													"style" : "",
													"text" : "set $1"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-29",
													"maxclass" : "newobj",
													"numinlets" : 1,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 50.0, 141.0, 65.0, 22.0 ],
													"style" : "",
													"text" : "prepend 1"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-27",
													"maxclass" : "message",
													"numinlets" : 2,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 151.5, 141.0, 43.0, 22.0 ],
													"style" : "",
													"text" : "set $1"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-25",
													"maxclass" : "newobj",
													"numinlets" : 1,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 152.0, 186.0, 115.0, 22.0 ],
													"style" : "",
													"text" : "prepend quant_grid"
												}

											}
, 											{
												"box" : 												{
													"format" : 6,
													"id" : "obj-18",
													"maxclass" : "flonum",
													"maximum" : 1.0,
													"minimum" : 0.0,
													"numinlets" : 1,
													"numoutlets" : 2,
													"outlettype" : [ "", "bang" ],
													"parameter_enable" : 0,
													"patching_rect" : [ 463.0, 30.0, 50.0, 22.0 ],
													"presentation" : 1,
													"presentation_rect" : [ 159.0, 5.0, 50.0, 22.0 ],
													"style" : "",
													"varname" : "amount"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-15",
													"maxclass" : "number",
													"maximum" : 32,
													"minimum" : 0,
													"numinlets" : 1,
													"numoutlets" : 2,
													"outlettype" : [ "", "bang" ],
													"parameter_enable" : 0,
													"patching_rect" : [ 401.0, 30.0, 50.0, 22.0 ],
													"presentation" : 1,
													"presentation_rect" : [ -100.0, -100.0, 50.0, 22.0 ],
													"style" : "",
													"varname" : "offset"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-14",
													"maxclass" : "number",
													"maximum" : 8,
													"minimum" : 1,
													"numinlets" : 1,
													"numoutlets" : 2,
													"outlettype" : [ "", "bang" ],
													"parameter_enable" : 0,
													"patching_rect" : [ 338.0, 30.0, 50.0, 22.0 ],
													"presentation" : 1,
													"presentation_rect" : [ -100.0, -100.0, 50.0, 22.0 ],
													"style" : "",
													"varname" : "gridMultiplier"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-36",
													"items" : [ "none", ",", 24, ",", 32 ],
													"maxclass" : "umenu",
													"numinlets" : 1,
													"numoutlets" : 3,
													"outlettype" : [ "int", "", "" ],
													"parameter_enable" : 0,
													"patching_rect" : [ 227.5, 30.0, 100.0, 22.0 ],
													"presentation" : 1,
													"presentation_rect" : [ -100.0, 11.0, 100.0, 22.0 ],
													"style" : "",
													"varname" : "gridType"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-32",
													"items" : [ "quant_grid", ",", "quant_mult", ",", "quant_amount_node", ",", "quant_offset" ],
													"maxclass" : "umenu",
													"numinlets" : 1,
													"numoutlets" : 3,
													"outlettype" : [ "int", "", "" ],
													"parameter_enable" : 0,
													"patching_rect" : [ 111.0, 30.0, 100.0, 22.0 ],
													"presentation" : 1,
													"presentation_rect" : [ 6.0, 5.0, 116.0, 22.0 ],
													"style" : "",
													"varname" : "parameter"
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"destination" : [ "obj-29", 0 ],
													"source" : [ "obj-14", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-29", 0 ],
													"source" : [ "obj-15", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-29", 0 ],
													"source" : [ "obj-18", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-30", 0 ],
													"order" : 1,
													"source" : [ "obj-25", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-33", 1 ],
													"order" : 0,
													"source" : [ "obj-25", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-25", 0 ],
													"source" : [ "obj-27", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-29", 0 ],
													"source" : [ "obj-28", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-25", 0 ],
													"source" : [ "obj-29", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-39", 0 ],
													"source" : [ "obj-32", 1 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-28", 0 ],
													"source" : [ "obj-34", 1 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-29", 0 ],
													"source" : [ "obj-36", 1 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-35", 0 ],
													"source" : [ "obj-37", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-37", 0 ],
													"source" : [ "obj-38", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-40", 0 ],
													"source" : [ "obj-38", 1 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-41", 0 ],
													"source" : [ "obj-38", 3 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-42", 0 ],
													"source" : [ "obj-38", 2 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-27", 0 ],
													"source" : [ "obj-39", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-38", 0 ],
													"source" : [ "obj-39", 1 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-35", 0 ],
													"source" : [ "obj-40", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-35", 0 ],
													"source" : [ "obj-41", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-35", 0 ],
													"source" : [ "obj-42", 0 ]
												}

											}
 ]
									}
,
									"patching_rect" : [ 370.0, 60.5, 213.0, 36.0 ],
									"viewvisibility" : 1
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-53",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 376.5, 119.0, 209.0, 22.0 ],
									"presentation" : 1,
									"presentation_linecount" : 2,
									"presentation_rect" : [ 447.5, 573.0, 113.0, 36.0 ],
									"style" : "",
									"text" : "quant_amount_node 1 0."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-8",
									"maxclass" : "scope~",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 1033.0, 175.0, 130.0, 130.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 837.0, 229.0, 130.0, 130.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-9",
									"maxclass" : "scope~",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 895.333374, 175.0, 130.0, 130.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 699.333313, 229.0, 130.0, 130.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-7",
									"maxclass" : "scope~",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 757.666626, 175.0, 130.0, 130.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 561.666687, 229.0, 130.0, 130.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-24",
									"maxclass" : "scope~",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 620.0, 175.0, 130.0, 130.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 424.0, 229.0, 130.0, 130.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-11",
									"maxclass" : "scope~",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 1033.0, 321.0, 130.0, 130.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 852.0, 244.0, 130.0, 130.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-12",
									"maxclass" : "scope~",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 895.333374, 321.0, 130.0, 130.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 714.333313, 244.0, 130.0, 130.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-13",
									"maxclass" : "scope~",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 757.666626, 321.0, 130.0, 130.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 576.666687, 244.0, 130.0, 130.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-14",
									"maxclass" : "scope~",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 620.0, 321.0, 130.0, 130.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 439.0, 244.0, 130.0, 130.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"format" : 6,
									"id" : "obj-3",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 620.0, 67.5, 50.0, 22.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 424.0, 143.5, 50.0, 22.0 ],
									"style" : "",
									"varname" : "freq0"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-20",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 757.666626, 60.5, 325.333313, 20.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 546.666687, 101.5, 325.333313, 20.0 ],
									"style" : "",
									"text" : "Frequencies for nodes 1-3 (as multiple of root node)"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-19",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 614.5, 17.5, 429.0, 34.0 ],
									"presentation" : 1,
									"presentation_linecount" : 2,
									"presentation_rect" : [ 403.5, 58.5, 429.0, 34.0 ],
									"style" : "",
									"text" : "Frequency for node 0 \n(Base frequency of the network and the quantiser)"
								}

							}
, 							{
								"box" : 								{
									"format" : 6,
									"id" : "obj-5",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 1033.0, 87.5, 50.0, 22.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 822.0, 128.5, 50.0, 22.0 ],
									"style" : "",
									"varname" : "freq3"
								}

							}
, 							{
								"box" : 								{
									"format" : 6,
									"id" : "obj-6",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 895.333374, 87.5, 50.0, 22.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 684.333313, 128.5, 50.0, 22.0 ],
									"style" : "",
									"varname" : "freq2"
								}

							}
, 							{
								"box" : 								{
									"format" : 6,
									"id" : "obj-4",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 757.666626, 87.5, 50.0, 22.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 546.666687, 128.5, 50.0, 22.0 ],
									"style" : "",
									"varname" : "freq1"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-1",
									"maxclass" : "newobj",
									"numinlets" : 4,
									"numoutlets" : 8,
									"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal" ],
									"patching_rect" : [ 620.0, 128.5, 432.0, 22.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 409.0, 169.5, 432.0, 22.0 ],
									"style" : "",
									"text" : "cpg.network~ 4 1000 1 0 0 7 1 4.07 4.07"
								}

							}
, 							{
								"box" : 								{
									"angle" : 270.0,
									"bgcolor" : [ 0.952941, 0.564706, 0.098039, 0.45 ],
									"border" : 1,
									"bordercolor" : [ 0.870588, 0.415686, 0.062745, 1.0 ],
									"id" : "obj-57",
									"maxclass" : "panel",
									"mode" : 0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 752.333374, 53.5, 338.0, 65.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 541.333313, 94.5, 338.0, 65.0 ],
									"proportion" : 0.39,
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"angle" : 270.0,
									"bgcolor" : [ 0.960784, 0.827451, 0.156863, 0.45 ],
									"border" : 1,
									"bordercolor" : [ 0.764706, 0.592157, 0.101961, 1.0 ],
									"id" : "obj-56",
									"maxclass" : "panel",
									"mode" : 0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 610.5, 17.5, 479.833313, 101.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 399.5, 58.5, 479.833313, 101.0 ],
									"proportion" : 0.39,
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontface" : 1,
									"fontsize" : 16.0,
									"id" : "obj-28",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 13.0, 8.5, 257.0, 43.0 ],
									"presentation" : 1,
									"presentation_linecount" : 2,
									"presentation_rect" : [ 22.0, 49.5, 257.0, 43.0 ],
									"style" : "",
									"text" : "Central Pattern Generator network - triggers and quantiser"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-16",
									"linecount" : 28,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 13.0, 65.0, 345.0, 393.0 ],
									"presentation" : 1,
									"presentation_linecount" : 28,
									"presentation_rect" : [ 22.0, 106.0, 345.0, 393.0 ],
									"style" : "",
									"text" : "If argument 3 is set to 1, then cpg.network can generate triggers from the network behaviour - pulse signals that can be used to trigger notes, or other musical events. Triggers are generated at the first peak of each node's cycle.\n\nThere is also a built in quantiser, whose tempo is set by the cycle of node 0 (e.g. if node0 is cycling at 1hz, the quantiser will run at 60bpm, with each bar starting when node 0 \"fires\". \n\nThis quantiser effect is variable from no quantisation (0 - the raw timing from the network) to full quantisation (1 - all notes are moved to the nearest gridline), or somewhere in between (moved somewhat towards the gridline)\n\nA different quantiser grid can be specified for each node. The grid can be 32 or 24 units long (4/4 or 3/4) and a multiplier sets the grid unit size. e.g. to quantise to quarter notes, the grid would be set to 32, and the multiplier to 8 - for 4 blocks of 8 (32/8=4), to quantise to 8th notes, the multiplier would be changed to 4 (32/4=8)\n\nThe amount of quantisation is also variable per node.\nFinally an offset (0-32 or 0-24) can be applied to the quantiser, to allow patterns to be shifted in time\n\nThe quantiser cannot be applied to node zero - since this node drives the quantiser tempo - its note trigger defines the first beat of the bar"
								}

							}
, 							{
								"box" : 								{
									"angle" : 270.0,
									"bgcolor" : [ 1.0, 1.0, 1.0, 0.45 ],
									"border" : 1,
									"bordercolor" : [ 0.239216, 0.254902, 0.278431, 1.0 ],
									"id" : "obj-10",
									"maxclass" : "panel",
									"mode" : 0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 11.0, 8.5, 346.0, 512.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 16.5, 43.5, 359.0, 517.0 ],
									"proportion" : 0.39,
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"angle" : 270.0,
									"bgcolor" : [ 0.317647, 0.654902, 0.976471, 0.45 ],
									"border" : 1,
									"bordercolor" : [ 0.011765, 0.396078, 0.752941, 1.0 ],
									"id" : "obj-65",
									"maxclass" : "panel",
									"mode" : 0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 370.0, 17.5, 229.833313, 149.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 423.0, 367.5, 232.833313, 217.0 ],
									"proportion" : 0.39,
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"angle" : 270.0,
									"bgcolor" : [ 0.439216, 0.74902, 0.254902, 0.45 ],
									"border" : 1,
									"bordercolor" : [ 0.0, 0.533333, 0.168627, 1.0 ],
									"id" : "obj-66",
									"maxclass" : "panel",
									"mode" : 0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 370.0, 179.0, 229.833313, 272.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 691.5, 397.5, 309.0, 217.0 ],
									"proportion" : 0.39,
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"angle" : 270.0,
									"bgcolor" : [ 1.0, 1.0, 1.0, 0.45 ],
									"border" : 1,
									"bordercolor" : [ 0.239216, 0.254902, 0.278431, 1.0 ],
									"id" : "obj-55",
									"maxclass" : "panel",
									"mode" : 0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 370.0, 458.0, 793.0, 62.5 ],
									"presentation" : 1,
									"presentation_rect" : [ 31.5, 58.5, 359.0, 517.0 ],
									"proportion" : 0.39,
									"style" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-11", 0 ],
									"midpoints" : [ 1042.5, 158.0, 609.0, 158.0, 609.0, 309.0, 1042.5, 309.0 ],
									"source" : [ "obj-1", 7 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-12", 0 ],
									"midpoints" : [ 983.5, 158.0, 608.0, 158.0, 608.0, 309.0, 904.833374, 309.0 ],
									"source" : [ "obj-1", 6 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-13", 0 ],
									"midpoints" : [ 924.5, 156.0, 608.0, 156.0, 608.0, 308.0, 767.166626, 308.0 ],
									"source" : [ "obj-1", 5 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-14", 0 ],
									"midpoints" : [ 865.5, 157.0, 609.0, 157.0, 609.0, 313.0, 629.5, 313.0 ],
									"source" : [ "obj-1", 4 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-24", 0 ],
									"source" : [ "obj-1", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-7", 0 ],
									"midpoints" : [ 688.5, 166.0, 767.166626, 166.0 ],
									"source" : [ "obj-1", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-8", 0 ],
									"midpoints" : [ 806.5, 164.0, 1042.5, 164.0 ],
									"source" : [ "obj-1", 3 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-9", 0 ],
									"midpoints" : [ 747.5, 165.0, 904.833374, 165.0 ],
									"source" : [ "obj-1", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-104", 0 ],
									"hidden" : 1,
									"source" : [ "obj-107", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"source" : [ "obj-3", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-48", 0 ],
									"midpoints" : [ 508.0, 354.0, 389.0, 354.0 ],
									"source" : [ "obj-30", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"midpoints" : [ 379.5, 116.0, 372.0, 116.0, 372.0, 152.0, 606.0, 152.0, 606.0, 122.0, 629.5, 122.0 ],
									"order" : 0,
									"source" : [ "obj-37", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-53", 1 ],
									"midpoints" : [ 379.5, 106.5, 576.0, 106.5 ],
									"order" : 1,
									"source" : [ "obj-37", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 1 ],
									"source" : [ "obj-4", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-48", 0 ],
									"source" : [ "obj-40", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-40", 0 ],
									"source" : [ "obj-47", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"midpoints" : [ 389.0, 392.0, 606.0, 392.0, 606.0, 123.0, 629.5, 123.0 ],
									"order" : 0,
									"source" : [ "obj-48", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-44", 1 ],
									"midpoints" : [ 389.0, 391.5, 483.0, 391.5 ],
									"order" : 1,
									"source" : [ "obj-48", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 3 ],
									"source" : [ "obj-5", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 2 ],
									"source" : [ "obj-6", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 1157.0, 5.0, 61.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "",
					"text" : "p triggers",
					"varname" : "triggers"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-15",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 639.5, 306.0, 261.0, 20.0 ],
					"presentation" : 1,
					"presentation_linecount" : 2,
					"presentation_rect" : [ 417.5, 356.0, 220.0, 34.0 ],
					"style" : "",
					"text" : "Set signal offset of connection between nodes"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-18",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 775.833374, 332.0, 36.0, 20.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 553.833374, 382.0, 36.0, 20.0 ],
					"style" : "",
					"text" : "to"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-21",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 712.166687, 332.0, 36.0, 20.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 490.166687, 382.0, 36.0, 20.0 ],
					"style" : "",
					"text" : "from"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-23",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 639.5, 332.0, 44.0, 20.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 417.5, 382.0, 44.0, 20.0 ],
					"style" : "",
					"text" : "offset"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-25",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 639.5, 493.0, 106.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 417.5, 543.0, 113.0, 22.0 ],
					"style" : "",
					"text" : "offset_conn 0 0 0."
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-26",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 712.166687, 393.0, 30.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 490.166687, 443.0, 30.0, 22.0 ],
					"style" : "",
					"text" : "pak"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-27",
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 773.166687, 354.0, 50.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 551.166687, 404.0, 50.0, 22.0 ],
					"style" : "",
					"varname" : "weightto[1]"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-29",
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 712.166687, 354.0, 50.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 490.166687, 404.0, 50.0, 22.0 ],
					"style" : "",
					"varname" : "weightfrom[1]"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-31",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 712.166687, 420.0, 125.0, 22.0 ],
					"presentation" : 1,
					"presentation_linecount" : 2,
					"presentation_rect" : [ 490.166687, 470.0, 98.0, 36.0 ],
					"style" : "",
					"text" : "set offset_conn $1 $2"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-32",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 639.5, 456.0, 140.0, 22.0 ],
					"presentation" : 1,
					"presentation_linecount" : 2,
					"presentation_rect" : [ 417.5, 506.0, 113.0, 36.0 ],
					"style" : "",
					"text" : "prepend offset_conn 0 1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-33",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 639.5, 354.0, 50.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 417.5, 404.0, 50.0, 22.0 ],
					"style" : "",
					"varname" : "weight[1]"
				}

			}
, 			{
				"box" : 				{
					"angle" : 270.0,
					"bgcolor" : [ 0.701961, 0.415686, 0.886275, 0.45 ],
					"border" : 1,
					"bordercolor" : [ 0.466667, 0.254902, 0.607843, 1.0 ],
					"id" : "obj-34",
					"maxclass" : "panel",
					"mode" : 0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 630.0, 302.5, 283.833313, 217.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 408.0, 352.5, 232.833313, 217.0 ],
					"proportion" : 0.39,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"hidden" : 1,
					"id" : "obj-2",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 4,
					"outlettype" : [ "", "", "", "" ],
					"patching_rect" : [ 937.0, 88.5, 58.0, 22.0 ],
					"restore" : 					{
						"freq0" : [ 2.0 ],
						"freq1" : [ 1.0 ],
						"freq2" : [ 2.0 ],
						"freq3" : [ 4.0 ],
						"number" : [ 4.0 ],
						"number[1]" : [ 1.0 ],
						"number[2]" : [ 4.0 ],
						"number[3]" : [ 4.0 ],
						"weight" : [ 0.0 ],
						"weight[1]" : [ 0.0 ],
						"weightfrom" : [ 0 ],
						"weightfrom[1]" : [ 0 ],
						"weightto" : [ 1 ],
						"weightto[1]" : [ 0 ]
					}
,
					"style" : "",
					"text" : "autopattr",
					"varname" : "u886007625"
				}

			}
, 			{
				"box" : 				{
					"hidden" : 1,
					"id" : "obj-107",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 937.0, 8.5, 72.0, 22.0 ],
					"style" : "",
					"text" : "loadmess 1"
				}

			}
, 			{
				"box" : 				{
					"hidden" : 1,
					"id" : "obj-104",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 937.0, 55.5, 187.0, 22.0 ],
					"saved_object_attributes" : 					{
						"client_rect" : [ 1057, 45, 1440, 407 ],
						"parameter_enable" : 0,
						"storage_rect" : [ 583, 69, 1034, 197 ]
					}
,
					"style" : "",
					"text" : "pattrstorage basic @savemode 2",
					"varname" : "basic"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-89",
					"linecount" : 3,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 1080.0, 341.0, 155.0, 48.0 ],
					"presentation" : 1,
					"presentation_linecount" : 3,
					"presentation_rect" : [ 785.5, 376.0, 155.0, 48.0 ],
					"style" : "",
					"text" : "some parameter settings\nare unstable and the oscillator may stop."
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-87",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 1076.666748, 431.0, 21.0, 20.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 782.166748, 466.0, 21.0, 20.0 ],
					"style" : "",
					"text" : "G"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-85",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 1050.916748, 404.0, 20.083313, 20.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 756.416748, 439.0, 20.083313, 20.0 ],
					"style" : "",
					"text" : "B"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-83",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 1017.916748, 377.0, 31.0, 20.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 723.416748, 412.0, 31.0, 20.0 ],
					"style" : "",
					"text" : "C"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-81",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 990.666687, 348.0, 67.0, 20.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 696.166687, 383.0, 67.0, 20.0 ],
					"style" : "",
					"text" : "ratio T1:T2"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-78",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 1021.0, 431.0, 50.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 726.5, 466.0, 50.0, 22.0 ],
					"style" : "",
					"varname" : "number[3]"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-79",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 990.666687, 404.0, 50.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 696.166687, 439.0, 50.0, 22.0 ],
					"style" : "",
					"varname" : "number[2]"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-77",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 960.333313, 377.0, 50.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 665.833313, 412.0, 50.0, 22.0 ],
					"style" : "",
					"varname" : "number[1]"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-75",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 930.0, 348.0, 50.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 635.5, 383.0, 50.0, 22.0 ],
					"style" : "",
					"varname" : "number"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-73",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1128.416626, 400.0, 37.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 833.916626, 435.0, 37.0, 22.0 ],
					"style" : "",
					"text" : "reset"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-69",
					"maxclass" : "newobj",
					"numinlets" : 4,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 930.0, 464.0, 110.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 635.5, 499.0, 110.0, 22.0 ],
					"style" : "",
					"text" : "pak 4. 1. 4.07 4.07"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-68",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 930.0, 492.0, 117.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 635.5, 527.0, 117.0, 22.0 ],
					"style" : "",
					"text" : "params $1 $2 $3 $4"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-64",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 389.5, 306.0, 218.0, 20.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 402.5, 341.0, 218.0, 20.0 ],
					"style" : "",
					"text" : "Set signal connection between nodes"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-62",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 525.833374, 332.0, 36.0, 20.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 538.833374, 367.0, 36.0, 20.0 ],
					"style" : "",
					"text" : "to"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-61",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 462.166687, 332.0, 36.0, 20.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 475.166687, 367.0, 36.0, 20.0 ],
					"style" : "",
					"text" : "from"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-59",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 389.5, 332.0, 44.0, 20.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 402.5, 367.0, 44.0, 20.0 ],
					"style" : "",
					"text" : "weight"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-53",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 389.5, 493.0, 113.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 402.5, 528.0, 113.0, 22.0 ],
					"style" : "",
					"text" : "weight 0 1 0."
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-46",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 462.166687, 393.0, 30.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 475.166687, 428.0, 30.0, 22.0 ],
					"style" : "",
					"text" : "pak"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-45",
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 523.166687, 354.0, 50.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 536.166687, 389.0, 50.0, 22.0 ],
					"style" : "",
					"varname" : "weightto"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-43",
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 462.166687, 354.0, 50.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 475.166687, 389.0, 50.0, 22.0 ],
					"style" : "",
					"varname" : "weightfrom"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-40",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 462.166687, 420.0, 98.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 475.166687, 455.0, 98.0, 22.0 ],
					"style" : "",
					"text" : "set weight $1 $2"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-37",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 389.5, 456.0, 113.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 402.5, 491.0, 113.0, 22.0 ],
					"style" : "",
					"text" : "prepend weight 0 1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-30",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 389.5, 354.0, 50.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 402.5, 389.0, 50.0, 22.0 ],
					"style" : "",
					"varname" : "weight"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-22",
					"linecount" : 10,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 940.5, 131.5, 286.0, 144.0 ],
					"presentation" : 1,
					"presentation_linecount" : 10,
					"presentation_rect" : [ 953.5, 166.5, 286.0, 144.0 ],
					"style" : "",
					"text" : "arguments:\n1. No of Nodes\n2. sample rate (hz)\n3. generate note triggers? (0/1)\n4. external signal inlets? (0/1)\n5. synchronise to external signal? (0/1/2 - see tab)\n6. equation param: ratio of T1:T2\n7. equation param: C\n8. equation param: B\n9. equation param: G"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-20",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 518.666687, 51.5, 325.333313, 20.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 531.666687, 86.5, 325.333313, 20.0 ],
					"style" : "",
					"text" : "Frequencies for nodes 1-3 (as multiple of root node)"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-19",
					"linecount" : 2,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 375.5, 8.5, 429.0, 34.0 ],
					"presentation" : 1,
					"presentation_linecount" : 2,
					"presentation_rect" : [ 388.5, 43.5, 429.0, 34.0 ],
					"style" : "",
					"text" : "Frequency for node 0 \n(Base frequency of the network - changing this changes all frequencies)"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-17",
					"linecount" : 2,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 926.0, 302.5, 319.0, 34.0 ],
					"presentation" : 1,
					"presentation_linecount" : 2,
					"presentation_rect" : [ 631.5, 337.5, 319.0, 34.0 ],
					"style" : "",
					"text" : "Equation parameters. See cpg.freq for more details.\nChanges cause drop-out in audio as system recalibrates\n"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 1,
					"fontsize" : 16.0,
					"id" : "obj-28",
					"linecount" : 2,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 13.0, 8.5, 257.0, 43.0 ],
					"presentation" : 1,
					"presentation_linecount" : 2,
					"presentation_rect" : [ 22.0, 49.5, 257.0, 43.0 ],
					"style" : "",
					"text" : "Central Pattern Generator network"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-16",
					"linecount" : 36,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 13.0, 65.0, 337.0, 503.0 ],
					"presentation" : 1,
					"presentation_linecount" : 36,
					"presentation_rect" : [ 22.0, 106.0, 337.0, 503.0 ],
					"style" : "",
					"text" : "This is another step on from cpg.node_freq - a network of CPG nodes in a single object. Like cpg.node_freq, both the equation parameters, and the frequencies of nodes can be set. All connections in the network begin with their weight set to 0\n\nThis object also adds a number of other features to allow you to manipulate network behaviour\n- Adjust signal connections between nodes\n- Generate note-triggers from node behaviour (with or without quantisation)\n- Apply delays to node outputs, and to signal connections between nodes\n- Set \"internal noise\" level for each node\n- Apply delay to individual node outputs\n\n\nThese features mean the object requires more CPU power than the other cpg. objects. For CPU efficiency, the sample rate of the object can be set to a lower rate than the overall max environment. We have found 1000 hz is a good trade-off for control-signal applications. Of course the sample rate can be set to (e.g.) 44100 if you wish to listen to the audio output of the network. Below a signal rates of 44100 interpolation is used we have found this completely adequate for control signal use, but less so for audio (though you may like the digital crunchiness).\n\n\nThese objects develop on this project\nhttp://www.nime.org/proceedings/2018/nime2018_paper0047.pdf\nPlease cite that paper if you use these objects in your own work\n\nmy email: db15237@bristol.ac.uk"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-8",
					"maxclass" : "scope~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 794.0, 164.0, 130.0, 130.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 807.0, 199.0, 130.0, 130.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-9",
					"maxclass" : "scope~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 656.333313, 164.0, 130.0, 130.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 669.333313, 199.0, 130.0, 130.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-7",
					"maxclass" : "scope~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 518.666687, 164.0, 130.0, 130.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 531.666687, 199.0, 130.0, 130.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-24",
					"maxclass" : "scope~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 381.0, 164.0, 130.0, 130.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 394.0, 199.0, 130.0, 130.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-5",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 794.0, 78.5, 50.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 807.0, 113.5, 50.0, 22.0 ],
					"style" : "",
					"varname" : "freq3"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-6",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 656.333313, 78.5, 50.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 669.333313, 113.5, 50.0, 22.0 ],
					"style" : "",
					"varname" : "freq2"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-4",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 518.666687, 78.5, 50.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 531.666687, 113.5, 50.0, 22.0 ],
					"style" : "",
					"varname" : "freq1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-3",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 381.0, 51.5, 50.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 394.0, 113.5, 50.0, 22.0 ],
					"style" : "",
					"varname" : "freq0"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-1",
					"maxclass" : "newobj",
					"numinlets" : 4,
					"numoutlets" : 4,
					"outlettype" : [ "signal", "signal", "signal", "signal" ],
					"patching_rect" : [ 381.0, 119.5, 432.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 394.0, 154.5, 432.0, 22.0 ],
					"style" : "",
					"text" : "cpg.network~ 4 1000 0 0 0 7 1 4.07 4.07"
				}

			}
, 			{
				"box" : 				{
					"angle" : 270.0,
					"bgcolor" : [ 0.952941, 0.564706, 0.098039, 0.45 ],
					"border" : 1,
					"bordercolor" : [ 0.870588, 0.415686, 0.062745, 1.0 ],
					"id" : "obj-57",
					"maxclass" : "panel",
					"mode" : 0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 513.333313, 44.5, 338.0, 65.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 526.333313, 79.5, 338.0, 65.0 ],
					"proportion" : 0.39,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"angle" : 270.0,
					"bgcolor" : [ 0.960784, 0.827451, 0.156863, 0.45 ],
					"border" : 1,
					"bordercolor" : [ 0.764706, 0.592157, 0.101961, 1.0 ],
					"id" : "obj-56",
					"maxclass" : "panel",
					"mode" : 0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 371.5, 8.5, 479.833313, 101.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 384.5, 43.5, 479.833313, 101.0 ],
					"proportion" : 0.39,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"angle" : 270.0,
					"bgcolor" : [ 0.439216, 0.74902, 0.254902, 0.45 ],
					"border" : 1,
					"bordercolor" : [ 0.0, 0.533333, 0.168627, 1.0 ],
					"id" : "obj-66",
					"maxclass" : "panel",
					"mode" : 0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 926.0, 302.5, 309.0, 217.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 631.5, 337.5, 309.0, 217.0 ],
					"proportion" : 0.39,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"angle" : 270.0,
					"bgcolor" : [ 0.317647, 0.654902, 0.976471, 0.45 ],
					"border" : 1,
					"bordercolor" : [ 0.011765, 0.396078, 0.752941, 1.0 ],
					"id" : "obj-65",
					"maxclass" : "panel",
					"mode" : 0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 380.0, 302.5, 232.833313, 217.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 393.0, 337.5, 232.833313, 217.0 ],
					"proportion" : 0.39,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"angle" : 270.0,
					"bgcolor" : [ 1.0, 1.0, 1.0, 0.45 ],
					"border" : 1,
					"bordercolor" : [ 0.239216, 0.254902, 0.278431, 1.0 ],
					"id" : "obj-10",
					"maxclass" : "panel",
					"mode" : 0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 11.0, 8.5, 352.0, 564.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 16.5, 43.5, 359.0, 517.0 ],
					"proportion" : 0.39,
					"style" : ""
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-24", 0 ],
					"source" : [ "obj-1", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-7", 0 ],
					"source" : [ "obj-1", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-8", 0 ],
					"source" : [ "obj-1", 3 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-9", 0 ],
					"source" : [ "obj-1", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"hidden" : 1,
					"source" : [ "obj-107", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 0 ],
					"source" : [ "obj-26", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-26", 1 ],
					"midpoints" : [ 782.666687, 388.0, 732.666687, 388.0 ],
					"source" : [ "obj-27", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-26", 0 ],
					"source" : [ "obj-29", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"source" : [ "obj-3", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-37", 0 ],
					"source" : [ "obj-30", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 0 ],
					"midpoints" : [ 721.666687, 449.0, 649.166687, 449.0, 649.166687, 451.0, 649.0, 451.0 ],
					"source" : [ "obj-31", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"midpoints" : [ 649.0, 485.0, 621.0, 485.0, 621.0, 533.0, 369.0, 533.0, 369.0, 116.0, 390.5, 116.0 ],
					"order" : 1,
					"source" : [ "obj-32", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-25", 1 ],
					"midpoints" : [ 649.0, 485.0, 736.0, 485.0 ],
					"order" : 0,
					"source" : [ "obj-32", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 0 ],
					"source" : [ "obj-33", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"midpoints" : [ 399.0, 486.0, 368.0, 486.0, 368.0, 117.0, 390.5, 117.0 ],
					"order" : 1,
					"source" : [ "obj-37", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-53", 1 ],
					"midpoints" : [ 399.0, 485.0, 493.0, 485.0 ],
					"order" : 0,
					"source" : [ "obj-37", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 1 ],
					"source" : [ "obj-4", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-37", 0 ],
					"midpoints" : [ 471.666687, 449.0, 399.166687, 449.0, 399.166687, 451.0, 399.0, 451.0 ],
					"source" : [ "obj-40", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-46", 0 ],
					"source" : [ "obj-43", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-46", 1 ],
					"midpoints" : [ 532.666687, 388.0, 482.666687, 388.0 ],
					"source" : [ "obj-45", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-40", 0 ],
					"source" : [ "obj-46", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 3 ],
					"source" : [ "obj-5", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 2 ],
					"source" : [ "obj-6", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"midpoints" : [ 939.5, 533.0, 368.0, 533.0, 368.0, 117.0 ],
					"source" : [ "obj-68", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-68", 0 ],
					"source" : [ "obj-69", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"midpoints" : [ 1137.916626, 533.0, 368.0, 533.0, 368.0, 117.0, 390.5, 117.0 ],
					"source" : [ "obj-73", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-69", 0 ],
					"source" : [ "obj-75", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-69", 1 ],
					"source" : [ "obj-77", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-69", 3 ],
					"source" : [ "obj-78", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-69", 2 ],
					"source" : [ "obj-79", 0 ]
				}

			}
 ],
		"dependency_cache" : [ 			{
				"name" : "quantselecta.maxpat",
				"bootpath" : "D:/edu/msc/matsuokaMax/helpPatchNetwork",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "cpg.network~.mxe64",
				"type" : "mx64"
			}
 ],
		"autosave" : 0
	}

}
