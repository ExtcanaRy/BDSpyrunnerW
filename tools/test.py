import mc

# some tool functions
def list_to_str(list: list):
	result = ", ".join([str(element) for element in list])
	return f"({result})"

# print variable type
def pVT(variable): 
	return type(variable)[:1]

logger = mc.Logger(__name__)

# entity test results
'''
variable prefix definition:
	s_: string
	i_: int
	l_: list
	b_: bool
	f_: float
	e_: entity

Entity的属性
name:str - 实体名字                          # test
uuid:str - 玩家UUID                          # test
xuid:str - 玩家XUID                          # test  
pos:list - 实体坐标                          # test
did:int - 实体维度ID                         # test
is_standing:bool - 实体是否站立在方块上       # test
is_sneaking:bool - 实体是否潜行              # test
health:int - 实体当前生命                    # test
maxhealth:int - 实体最大生命                 # test
perm:int - 玩家权限值                        # test
platform_online_id:str - 玩家设备ID          # empty string
platform:int - 玩家设备操作系统               # test
IP:str - 玩家IP                              # test

Problematic but available listeners:
onPistonPush
'''



# test functions
def testonConsoleCmd(cmd): #testonConsoleInput
	logger.debug(f"s_cmd = {cmd}", info="onConsoleInput")
#mc.setListener('onConsoleCmd', testonConsoleCmd) # onConsoleInput -> onConsoleCmd
# status: pass

def testonConsoleOutput(output):
	logger.debug(f"s_output = {output}", info="onConsoleOutput")
#mc.setListener('onConsoleOutput', testonConsoleOutput)
# status: pass

def testonPreJoin(player): #testonPlayerJoin
	logger.debug(f"s_player.name = {player.name}", info="onPreJoin")
#mc.setListener('onPreJoin', testonPreJoin)
# status: pass

def testonJoin(player): #testonPlayerJoin
	mc.getPlayerList()[0].sendCustomForm('{"content":[{"type":"label","text":"114514"}], "type":"custom_form","title":"test"}')
	logger.debug(f"s_player.uuid = {player.uuid} s_player.IP {player.IP}", info="onJoin")
#mc.setListener('onJoin', testonJoin) #onPlayerJoin -> onJoin
# status: pass

def testonLeft(player): #testonPlayerLeft
	logger.debug(f"s_player.name = {player.name} s_player.xuid = {player.xuid}", info="onLeft")
#mc.setListener('onLeft', testonLeft) #onPlayerLeft -> onLeft
# status: pass

def testonPlayerAttack(e):
	logger.debug(f"l_e['player'].pos = {list_to_str(e['player'].pos)}, e['actor'].health = {e['actor'].health}", info="onPlayerAttack")
	playerList = mc.getPlayerList()
	logger.debug("mcAPI l_getPlayerList = %s" % (playerList))
#mc.setListener('onPlayerAttack', testonPlayerAttack)
# status: pass

def testonFormSelected(e): #testonSelectForm
	logger.debug(f"i_e['formid'] = {e['formid']}, l_e['selected'] = {e['selected']}, s_e['player'].name = {e['player'].name}", info="onFormSelected")
	e['player'].teleport(0,100,0,0)
#mc.setListener("onFormSelected", testonFormSelected)
# status: pass
# import mc
# mc.getPlayerList()[0].sendCustomForm('{"content":[{"type":"label","text":"114514"}], "type":"custom_form","title":"test"}')

def testonUseItem(e):
	logger.debug(f"i_e['itemid'] = {e['itemid']}, i_e['player'].did = {e['player'].did}", info="onUseItem")
#mc.setListener("onUseItem", testonUseItem)
# status: pass

def testonUseItemEx(e):
	logger.debug(f"i_e['itemid'] = {e['itemid']}, i_e['player'].pos = {list_to_str(e['player'].pos)}", info="onUseItemEx")
#mc.setListener("onUseItemEx", testonUseItemEx)
# status: pass

def testonPlaceBlock(e):
	logger.debug(f"i_e['blockid'] = {e['blockid']}, b_e['player'].is_standing = {e['player'].is_standing}", info="onPlaceBlock") # Entity.isstand -> Entity.is_standing
#mc.setListener("onPlaceBlock", testonPlaceBlock)
# status: pass

def testonPlacedBlock(e):
	logger.debug(f"i_e['blockid'] = {e['blockid']}, s_e['player'].name = {e['player'].name}", info="onPlacedBlock") # Entity.isstand -> Entity.is_standing
#mc.setListener("onPlacedBlock", testonPlacedBlock)
# status: pass

def testonDestroyBlock(e):
	logger.debug(f"l_e['position'] = {list_to_str(e['position'])}, {e['position'][1]}, {e['position'][2]}, b_e['player'].is_sneaking = {e['player'].is_sneaking}", info="onDestroyBlock") # issneak -> is_sneaking
#mc.setListener("onDestroyBlock", testonDestroyBlock)
# status: pass

def testonDestroyedBlock(e):
	logger.debug(f"l_e['position'] = {list_to_str(e['position'])}, {e['position'][1]}, {e['position'][2]}, b_e['player'].is_sneaking = {e['player'].is_sneaking}", info="onDestroyedBlock") # issneak -> is_sneaking
#mc.setListener("onDestroyedBlock", testonDestroyedBlock)
# status: pass

def testonOpenContainer(e):
	logger.debug(f"l_e['position'] = {list_to_str(e['position'])}, i_e['player'].typeid = {e['player'].typeid}", info="onOpenContainer")
#mc.setListener("onOpenContainer", testonOpenContainer)
# status: pass

def testonCloseContainer(e):
	logger.debug(f"l_e['position'] = {list_to_str(e['position'])}, s_e['player'].typename = e['player'].typename)", info="onCloseContainer")
#mc.setListener("onCloseContainer", testonCloseContainer)
# status: pass

def testonContainerChange(e):
	logger.debug('l_e["position"] = (%f,%f,%f), i_itemid = %d, i_itemcount = %d, s_IP = %s' %
												(e['position'][0], e['position'][1], e['position'][2], e['itemid'], e['itemcount'], e['player'].IP), info="onContainerChange")
#mc.setListener("onContainerChange", testonContainerChange)
# status: pass

def testonChangeDim(player): #testonChangeDimension
	logger.debug('s_playerDim = %s' % (player.did), info="onChangeDim")
#mc.setListener("onChangeDim", testonChangeDim)
# status: pass

def testonMobDie(e):
	# if e["actor2"]:
	try:
		logger.debug('i_e["dmcase"] = %s, e_e["actor1"] = %s, i_e["actor2"].maxhealth = %s' % (e["dmcase"], e["actor1"], e["actor2"].maxhealth), info="onMobDie")
	except:
		logger.debug('i_e["dmcase"] = %s, e_e["actor1"] = %s, i_e["actor2"] = %s' % (e["dmcase"], e["actor1"], e["actor2"]), info="onMobDie")
#mc.setListener("onMobDie", testonMobDie)
#actor1 always return nullptr
#return nullptr when use /kill command to kill mob
# status: pass

def testonMobHurt(e):
	# if e["actor2"]:
	try:
		logger.debug('i_e["dmcase"] = %s, i_e["actor1"].health = %s, s_e["actor2"].name = %s, i_e["damage"] = %s' % (e["dmcase"], e["actor1"].health, e["actor2"].name, e["damage"]), info="onMobHurt")
	except:
		logger.debug('i_e["dmcase"] = %s, i_e["actor1"].health = %s, s_e["actor2"] = %s, i_e["damage"] = %s' % (e["dmcase"], e["actor1"].health, e["actor2"], e["damage"]), info="onMobHurt")
#mc.setListener("onMobHurt", testonMobHurt)
# status: pass

def testonRespawn(player):
	logger.debug(f's_player.xuid = {player.xuid}', info="onRespawn")
#mc.setListener("onRespawn", testonRespawn)
# status: pass

def testonChat(e):
	logger.debug(f's_e["sender"] = {e["sender"]}, s_e["target"] = {e["target"]}, s_e["msg"] = {e["msg"]}, s_e["style"] = {e["style"]}', info="onChat")
#mc.setListener("onChat", testonChat)
# status: pass

def testonInputText(e):
	logger.debug(f's_e["msg"] = {e["msg"]}, s_e["player"].IP = {e["player"].IP}', info="onInputText")
#mc.setListener("onInputText", testonInputText)
# status: pass

def testonCommandBlockUpdate(e):
	logger.debug(f'i_e["player"].did = {e["player"].did}, s_e["cmd"] = {e["cmd"]}, s_e["mode"] = {e["mode"]}, s_e["condition"] = {e["condition"]}, s_e["redstone"] = {e["redstone"]}, s_e["output"] = {e["output"]}, s_e["rawname"] = {e["rawname"]}, s_e["delay"] = {e["delay"]}, l_e["position] = {e["position"]} ', info="onCommandBlockUpdate")
#mc.setListener("onCommandBlockUpdate", testonCommandBlockUpdate)
# status: pass

def testonPlayerCmd(e):
	logger.debug(f's_e["cmd"] = {e["cmd"]}, s_e["player"] = {e["player"]} ', info="onPlayerCmd")
#mc.setListener("onPlayerCmd", testonPlayerCmd)
# status: pass

def testonCmdBlockExecute(e):
	logger.debug(f's_e["cmd"] = {e["cmd"]}, s_e["rawname"] = {e["rawname"]}, l_e["position] = {e["position"]}, s_e["mode"] = {e["mode"]}, s_e["condition"] = {e["condition"]}', info="onCmdBlockExecute")
#mc.setListener("onCmdBlockExecute", testonCmdBlockExecute)
# status: pass

def testonLevelExplode(e):
	logger.debug(f'l_e["actor"].pos = {e["actor"].pos}, l_e["position"] = {e["position"]}, i_dimensionid = {e["dimensionid"]}, s_e["power"] = {e["power"]}', info="onLevelExplode")
#mc.setListener("onLevelExplode", testonLevelExplode)
# status: pass

def testonSetArmor(e):
	logger.debug(f's_e["player"].name = {e["player"].name}, i_e["itemid"] = {e["itemid"]}, i_e["itemcount"] = {e["itemcount"]}, s_e["itemname"] = {e["itemname"]}, s_e["itemaux"] = {e["itemaux"]}, s_e["slot"] = {e["slot"]}', info="onSetArmor")
#mc.setListener("onSetArmor", testonSetArmor)
# status: pass

def testonFarmLandDecay(e): 
	logger.debug(f'i_e["player"].platform_online_id = {e["player"].platform_online_id}, l_e["position"] = {e["position"]}, l_e["dimensionid"] = {e["dimensionid"]}', info="onFarmLandDecay")
#mc.setListener("onFarmLandDecay", testonFarmLandDecay)
# status: pass

def testonUseRespawnAnchor(e): #testonUseRespawnAnchorBlock
	logger.debug(f'i_e["player"].perm = {e["player"].perm}, l_e["position"] = {e["position"]}, i_e["dimensionid"] = {e["dimensionid"]}', info="onUseRespawnAnchor")
#mc.setListener("onUseRespawnAnchor", testonUseRespawnAnchor)
# status: pass

def testonScoreChanged(e):
	logger.debug(f'i_e["scoreboardid"] = {e["scoreboardid"]}, i_e["playersnum"] = {e["playersnum"]}, s_e["objectivename"] = {e["objectivename"]}, s_e["objectivedisname"] = {e["objectivedisname"]}', info="onScoreChanged")
#mc.setListener("onScoreChanged", testonScoreChanged)
# status: pass
# scripts:
#/scoreboard objectives add test dummy test
#/scoreboard players add @a test 114514

def testonMove(e):
	logger.debug(f'l_e.pos = {list_to_str(e.pos)}', info="onMove")
#mc.setListener("onMove", testonMove)
# status: pass

def testonPistonPush(e):
	logger.debug(f's_e["blockname"] = {e["blockname"]}, i_e["blockid"] = {e["blockid"]}, l_e["blockpos"] = {e["blockpos"]}, l_e["pistonpos"] = e["pistonpos"], i_e["dimensionid"] = {e["dimensionid"]}', info="onPistonPush")
#mc.setListener("onPistonPush", testonPistonPush)
# status: pass with problem
#key pistonpos will cause crash and it has been removed from dll plugin, please fix

def testonEndermanRandomTeleport(e):
	logger.debug(f'l_e.pos = {list_to_str(e.pos)}', info="onEndermanRandomTeleport")
#mc.setListener("onEndermanRandomTeleport", testonEndermanRandomTeleport)
# status: pass

def testonServerStarted(e):
	logger.debug(f'onServerStarted')
#mc.setListener("onServerStarted", testonServerStarted)
# status: pass

def testonDropItem(e):
	logger.debug(f's_e["player"].name = {e["player"].name}, i_e["itemid"] = {e["itemid"]}, i_e["itemcount"] = {e["itemcount"]}, s_e["itemname"] = {e["itemname"]}, i_e["itemcount"] = {e["itemaux"]}', info="onDropItem")
#mc.setListener("onDropItem", testonDropItem)
# status: pass

def testonTakeItem(e):
	logger.debug(f'b_e["player"].is_sneaking = {e["player"].is_sneaking}, l_e["actor"].pos = {list_to_str(e["actor"].pos)}', info="onTakeItem")
#mc.setListener("onTakeItem", testonTakeItem)
# status: psss

def testonRide(e):
	logger.debug(f's_e["actor1"].name = {e["actor1"].name}, s_e["actor2"].name = {e["actor2"].name}', info="onRide")
#mc.setListener("onRide", testonRide)
# status: pass

def testonUseFrameBlock(e):
	logger.debug(f's_e["player"].name = {e["player"].name}, l_e["blockpos"] = {list_to_str(e["blockpos"])}, i_e["dimensionid"] = {e["dimensionid"]}', info="onUseFrameBlock")
#mc.setListener("onUseFrameBlock", testonUseFrameBlock)
# status: pass

def testonJump(e):
	logger.debug(f'l_e.pos = {e.pos}', info="onJump")
#mc.setListener("onJump", testonJump)
# status: pass

def testonSneak(e):
	logger.debug(f's_e.platform_online_id = {e.platform_online_id}', info="onSneak")
#mc.setListener("onSneak", testonSneak)
# status: pass

def testonBlockInteracted(e):
	logger.debug(f's_e["player"].name = {e["player"].name}, l_e["blockpos"] = {list_to_str(e["blockpos"])}, s_e["blockname"] = {e["blockname"]}, i_e["blockid"] = {e["blockid"]}, i_e["dimensionid"] = {e["dimensionid"]}', info="onBlockInteracted")
#mc.setListener("onBlockInteracted", testonBlockInteracted)
# status: pass

def testonFireSpread(e):
	logger.debug(f'l_e["blockpos"] = {list_to_str(e["blockpos"])}, s_e["blockname"] = {e["blockname"]}, i_e["blockid"] = {e["blockid"]}', info="onFireSpread")
#mc.setListener("onFireSpread", testonFireSpread)
# status: pass

def testonBlockExploded(e):
	logger.debug(f's_e["actor"].name = {e["actor"].name}, l_e["blockpos"] = {list_to_str(e["blockpos"])}, s_e["blockname"] = {e["blockname"]}, i_e["blockid"] = {e["blockid"]}, i_e["dimensionid"] = {e["dimensionid"]}', info="onBlockExploded")
#mc.setListener("onBlockExploded", testonBlockExploded)
# status: pass

def testonUseSignBlock(e):
	logger.debug(f'i_e["player"].platform = {e["player"].platform}, s_e["text"] = {e["text"]}, l_e["position"] = {e["position"]}', info="onUseSignBlock")
#mc.setListener("onUseSignBlock", testonUseSignBlock)
# status: pass

def testonLiquidSpread(e):
	logger.debug(f's_e["src_name"] = {e["src_name"]}, l_e["src_pos"] = {e["src_pos"]}, s_e["dst_name"] = {e["dst_name"]}, l_e["dst_pos"] = {e["dst_pos"]}', info="onLiquidSpread")
# mc.setListener("onLiquidSpread", testonLiquidSpread)
# status: pass

#temple
def test(e):
	logger.debug(f'', info="Listener Name")
