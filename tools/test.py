import mc
import time

# some tool functions
def list_to_str(list: list):
	result = ", ".join([str(element) for element in list])
	return f"({result})"

def pVT(variable): # printVariableTypes
	return type(variable)[:1] 

def logout(*content, name: str = __name__, level: str = "INFO", info: str = ""):
    mc.log(content, name=name, level=level, info=info)

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
	logout("Listener onConsoleInput s_cmd = " + cmd)
	#mc.removeListener('onConsoleCmd', testonConsoleCmd)
#mc.setListener('onConsoleCmd', testonConsoleCmd) # onConsoleInput -> onConsoleCmd
# status: pass

def testonConsoleOutput(output):
	logout("Listener onConsoleOutput s_output = " + output)
	#mc.removeListener('onConsoleOutput', testonConsoleOutput)
#mc.setListener('onConsoleOutput', testonConsoleOutput)
# status: pass

def testonPreJoin(player): #testonPlayerJoin
	logout(f"Listener onPreJoin s_player.name = " + player.name)
	#mc.removeListener('onPreJoin', testonPreJoin)
#mc.setListener('onPreJoin', testonPreJoin)
# status: pass

def testonJoin(player): #testonPlayerJoin
	mc.getPlayerList()[0].sendCustomForm('{"content":[{"type":"label","text":"114514"}], "type":"custom_form","title":"test"}')
	logout(f"Listener onJoin s_player.uuid = " + player.uuid + " s_player.IP = " + player.IP)
	#mc.removeListener('onJoin', testonJoin)
#mc.setListener('onJoin', testonJoin) #onPlayerJoin -> onJoin
# status: pass

def testonLeft(player): #testonPlayerLeft
	logout("Listener onLeft s_player.name = " + str(player.name) + " s_player.xuid = " + str(player.xuid))
	#mc.removeListener('onLeft', testonLeft)
#mc.setListener('onLeft', testonLeft) #onPlayerLeft -> onLeft
# status: pass

def testonPlayerAttack(e):
	logout(f"Listener onPlayerAttack l_e['player'].pos = {list_to_str(e['player'].pos)}, e['actor'].health = {e['actor'].health}")
	playerList = mc.getPlayerList()
	logout("mcAPI l_getPlayerList = %s" % (playerList))
	#mc.removeListener('onPlayerAttack', testonPlayerAttack)
#mc.setListener('onPlayerAttack', testonPlayerAttack)
# status: pass+

def testonFormSelected(e): #testonSelectForm
	logout(f"Listener onFormSelected i_e['formid'] = {e['formid']}, l_e['selected'] = {e['selected']}, s_e['player'].name = {e['player'].name}")
	e['player'].teleport(0,100,0,0)
	#mc.removeListener('onFormSelected', testonFormSelected)
#mc.setListener("onFormSelected", testonFormSelected)
# status: pass
# import mc
# mc.getPlayerList()[0].sendCustomForm('{"content":[{"type":"label","text":"114514"}], "type":"custom_form","title":"test"}')

def testonUseItem(e):
	logout(f"Listener onUseItem i_e['itemid'] = {e['itemid']}, i_e['player'].did = {e['player'].did}")
	#mc.removeListener("onUseItem", testonUseItem)
#mc.setListener("onUseItem", testonUseItem)
# status: pass

def testonUseItemEx(e):
	logout(f"Listener onUseItemEx i_e['itemid'] = {e['itemid']}, i_e['player'].pos = {list_to_str(e['player'].pos)}")
	#mc.removeListener("onUseItemEx", testonUseItemEx)
#mc.setListener("onUseItemEx", testonUseItemEx)
# status: pass

def testonPlaceBlock(e):
	logout(f"Listener onPlaceBlock i_e['blockid'] = {e['blockid']}, b_e['player'].is_standing = {e['player'].is_standing}") # Entity.isstand -> Entity.is_standing
	#mc.removeListener("onPlaceBlock", testonPlaceBlock)
#mc.setListener("onPlaceBlock", testonPlaceBlock)
# status: pass

def testonPlacedBlock(e):
	logout(f"Listener onPlacedBlock i_e['blockid'] = {e['blockid']}, s_e['player'].name = {e['player'].name}") # Entity.isstand -> Entity.is_standing
	#mc.removeListener("onPlacedBlock", testonPlacedBlock)
#mc.setListener("onPlacedBlock", testonPlacedBlock)
# status: pass

def testonDestroyBlock(e):
	logout(f"Listener onDestroyBlock l_e['position'] = {list_to_str(e['position'])}, {e['position'][1]}, {e['position'][2]}）, b_e['player'].is_sneaking = {e['player'].is_sneaking}") # issneak -> is_sneaking
	#mc.removeListener("onDestroyBlock", testonDestroyBlock)
#mc.setListener("onDestroyBlock", testonDestroyBlock)
# status: pass

def testonOpenContainer(e):
	logout(f"Listener onOpenContainer l_e['position'] = {list_to_str(e['position'])}, i_e['player'].typeid = {e['player'].typeid}")
	#mc.removeListener("onOpenContainer", testonOpenContainer)
#mc.setListener("onOpenContainer", testonOpenContainer)
# status: pass

def testonCloseContainer(e):
	logout(f"Listener onCloseContainer l_e['position'] = {list_to_str(e['position'])}, s_e['player'].typename = e['player'].typename)")
	#mc.removeListener("onCloseContainer", testonCloseContainer)
#mc.setListener("onCloseContainer", testonCloseContainer)
# status: pass

def testonContainerChange(e):
	logout('Listener onContainerChange l_e["position"] = （%f,%f,%f）, i_itemid = %d, i_itemcount = %d, s_IP = %s' %
												(e['position'][0], e['position'][1], e['position'][2], e['itemid'], e['itemcount'], e['player'].IP))
	#mc.removeListener("onContainerChange", testonContainerChange)
#mc.setListener("onContainerChange", testonContainerChange)
# status: pass

def testonChangeDim(player): #testonChangeDimension
	logout('Listener onChangeDim s_playerDim = %s' % (player.did))
	#mc.removeListener("onChangeDim", testonChangeDim)
#mc.setListener("onChangeDim", testonChangeDim)
# status: pass

def testonMobDie(e):
	if e["actor2"]:
		logout('Listener onMobDie i_e["dmcase"] = %s, e_e["actor1"] = %s, i_e["actor2"].maxhealth = %s' % (e["dmcase"], e["actor1"], e["actor2"].maxhealth))
	else:
		logout('Listener onMobDie i_e["dmcase"] = %s, e_e["actor1"] = %s, i_e["actor2"] = %s' % (e["dmcase"], e["actor1"], e["actor2"]))
	#mc.removeListener("onMobDie", testonMobDie)
#mc.setListener("onMobDie", testonMobDie)
#actor1 always return nullptr
#return nullptr when use /kill command to kill mob
# status: pass

def testonMobHurt(e):
	if e["actor2"]:
		logout('Listener onMobHurt i_e["dmcase"] = %s, i_e["actor1"].health = %s, s_e["actor2"].name = %s, i_e["damage"] = %s' % (e["dmcase"], e["actor1"].health, e["actor2"].name, e["damage"]))
	else:
		logout('Listener onMobHurt i_e["dmcase"] = %s, i_e["actor1"].health = %s, s_e["actor2"] = %s, i_e["damage"] = %s' % (e["dmcase"], e["actor1"].health, e["actor2"], e["damage"]))
	##mc.removeListener("onMobHurt", testonMobHurt)
#mc.setListener("onMobHurt", testonMobHurt)
# status: pass

def testonRespawn(player):
	logout(f'Listener onRespawn s_player.xuid = {player.xuid}')
	#mc.removeListener("onRespawn", testonRespawn)
#mc.setListener("onRespawn", testonRespawn)
# status: pass

def testonChat(e):
	logout(f'Listener onChat s_e["sender"] = {e["sender"]}, s_e["target"] = {e["target"]}, s_e["msg"] = {e["msg"]}, s_e["style"] = {e["style"]}')
	#mc.removeListener("onChat", testonChat)
#mc.setListener("onChat", testonChat)
# status: pass

def testonInputText(e):
	logout(f'Listener onInputText s_e["msg"] = {e["msg"]}, s_e["player"].IP = {e["player"].IP}')
	#mc.removeListener("onInputText", testonInputText)
#mc.setListener("onInputText", testonInputText)
# status: pass

def testonCommandBlockUpdate(e):
	logout(f'Listener onCommandBlockUpdate i_e["player"].did = {e["player"].did}, s_e["cmd"] = {e["cmd"]}, s_e["mode"] = {e["mode"]}, s_e["condition"] = {e["condition"]}, s_e["redstone"] = {e["redstone"]}, s_e["output"] = {e["output"]}, s_e["rawname"] = {e["rawname"]}, s_e["delay"] = {e["delay"]}, l_e["position] = {e["position"]} ')
	#mc.removeListener("onCommandBlockUpdate", testonCommandBlockUpdate)
#mc.setListener("onCommandBlockUpdate", testonCommandBlockUpdate)
# status: pass

def testonPlayerCmd(e):
	logout(f'Listener onPlayerCmd s_e["cmd"] = {e["cmd"]}, s_e["player"] = {e["player"]} ')
	#mc.removeListener("onPlayerCmd", testonPlayerCmd)
#mc.setListener("onPlayerCmd", testonPlayerCmd)
# status: pass

def testonCmdBlockExecute(e):
	logout(f'Listener onCmdBlockExecute s_e["cmd"] = {e["cmd"]}, s_e["rawname"] = {e["rawname"]}, l_e["position] = {e["position"]}, s_e["mode"] = {e["mode"]}, s_e["condition"] = {e["condition"]}')
	#mc.removeListener("onCmdBlockExecute", testonCmdBlockExecute)
#mc.setListener("onCmdBlockExecute", testonCmdBlockExecute)
# status: pass

def testonLevelExplode(e):
	logout(f'Listener onLevelExplode l_e["actor"].pos = {e["actor"].pos}, l_e["position"] = {e["position"]}, i_dimensionid = {e["dimensionid"]}, s_e["power"] = {e["power"]}')
	#mc.removeListener("onLevelExplode", testonLevelExplode)
#mc.setListener("onLevelExplode", testonLevelExplode)
# status: pass

def testonSetArmor(e):
	logout(f'Listener onSetArmor s_e["player"].name = {e["player"].name}, i_e["itemid"] = {e["itemid"]}, i_e["itemcount"] = {e["itemcount"]}, s_e["itemname"] = {e["itemname"]}, s_e["itemaux"] = {e["itemaux"]}, s_e["slot"] = {e["slot"]}')
	#mc.removeListener("onSetArmor", testonSetArmor)
#mc.setListener("onSetArmor", testonSetArmor)
# status: pass

def testonFarmLandDecay(e): 
	logout(f'Listener onFarmLandDecay i_e["player"].platform_online_id = {e["player"].platform_online_id}, l_e["position"] = {e["position"]}, l_e["dimensionid"] = {e["dimensionid"]}')
	#mc.removeListener("onFarmLandDecay", testonFarmLandDecay)
#mc.setListener("onFarmLandDecay", testonFarmLandDecay)
# status: pass

def testonUseRespawnAnchor(e): #testonUseRespawnAnchorBlock
	logout(f'Listener onUseRespawnAnchor i_e["player"].perm = {e["player"].perm}, l_e["position"] = {e["position"]}, i_e["dimensionid"] = {e["dimensionid"]}')
	#mc.removeListener("onUseRespawnAnchor", testonUseRespawnAnchor)
#mc.setListener("onUseRespawnAnchor", testonUseRespawnAnchor)
# status: pass

def testonScoreChanged(e):
	logout(f'Listener onScoreChanged i_e["scoreboardid"] = {e["scoreboardid"]}, i_e["playersnum"] = {e["playersnum"]}, s_e["objectivename"] = {e["objectivename"]}, s_e["objectivedisname"] = {e["objectivedisname"]}')
	#mc.removeListener("onScoreChanged", testonScoreChanged)
#mc.setListener("onScoreChanged", testonScoreChanged)
# status: pass
# scripts:
#/scoreboard objectives add test dummy test
#/scoreboard players add @a test 114514

def testonMove(e):
	logout(f'Listener onMove l_e.pos = {list_to_str(e.pos)}')
	#mc.removeListener("onMove", testonMove)
#mc.setListener("onMove", testonMove)
# status: pass

def testonPistonPush(e):
	logout(f'Listener onPistonPush s_e["blockname"] = {e["blockname"]}, i_e["blockid"] = {e["blockid"]}, l_e["blockpos"] = {e["blockpos"]}, l_e["pistonpos"] = e["pistonpos"], i_e["dimensionid"] = {e["dimensionid"]}')
	#mc.removeListener("onPistonPush", testonPistonPush)
#mc.setListener("onPistonPush", testonPistonPush)
# status: pass with problem
#key pistonpos will cause crash and it has been removed from dll plugin, please fix

def testonEndermanRandomTeleport(e):
	logout(f'Listener onEndermanRandomTeleport l_e.pos = {list_to_str(e.pos)}')
	#mc.removeListener("onEndermanRandomTeleport", testonEndermanRandomTeleport)
#mc.setListener("onEndermanRandomTeleport", testonEndermanRandomTeleport)
# status: pass

def testonServerStarted(e):
	logout(f'Listener onServerStarted')
	#mc.removeListener("onServerStarted", testonServerStarted)
#mc.setListener("onServerStarted", testonServerStarted)
# status: pass

def testonDropItem(e):
	logout(f'Listener onDropItem s_e["player"].name = {e["player"].name}, i_e["itemid"] = {e["itemid"]}, i_e["itemcount"] = {e["itemcount"]}, s_e["itemname"] = {e["itemname"]}, i_e["itemcount"] = {e["itemaux"]}')
	#mc.removeListener("onDropItem", testonDropItem)
#mc.setListener("onDropItem", testonDropItem)
# status: pass

def testonTakeItem(e):
	logout(f'Listener onTakeItem b_e["player"].is_sneaking = {e["player"].is_sneaking}, l_e["actor"].pos = {list_to_str(e["actor"].pos)}')
	#mc.removeListener("onTakeItem", testonTakeItem)
#mc.setListener("onTakeItem", testonTakeItem)
# status: psss

def testonRide(e):
	logout(f'Listener onRide s_e["actor1"].name = {e["actor1"].name}, s_e["actor2"].name = {e["actor2"].name}')
	#mc.removeListener("onRide", testonRide)
#mc.setListener("onRide", testonRide)
# status: pass

def testonUseFrameBlock(e):
	logout(f'Listener onUseFrameBlock s_e["player"].name = {e["player"].name}, l_e["blockpos"] = {list_to_str(e["blockpos"])}, i_e["dimensionid"] = {e["dimensionid"]}')
	#mc.removeListener("onUseFrameBlock", testonUseFrameBlock)
#mc.setListener("onUseFrameBlock", testonUseFrameBlock)
# status: pass

def testonJump(e):
	logout(f'Listener onJump l_e.pos = {e.pos}')
	#mc.removeListener("onJump", testonJump)
#mc.setListener("onJump", testonJump)
# status: pass

def testonSneak(e):
	logout(f'Listener onSneak s_e.platform_online_id = {e.platform_online_id}')
	#mc.removeListener("onSneak", testonSneak)
#mc.setListener("onSneak", testonSneak)
# status: pass

def testonBlockInteracted(e):
	logout(f'Listener onBlockInteracted s_e["player"].name = {e["player"].name}, l_e["blockpos"] = {list_to_str(e["blockpos"])}, s_e["blockname"] = {e["blockname"]}, i_e["blockid"] = {e["blockid"]}, i_e["dimensionid"] = {e["dimensionid"]}')
	#mc.removeListener("onBlockInteracted", testonBlockInteracted)
#mc.setListener("onBlockInteracted", testonBlockInteracted)
# status: pass

def testonFireSpread(e):
	logout(f'Listener onFireSpread l_e["blockpos"] = {list_to_str(e["blockpos"])}, s_e["blockname"] = {e["blockname"]}, i_e["blockid"] = {e["blockid"]}')
	#mc.removeListener("onFireSpread", testonFireSpread)
#mc.setListener("onFireSpread", testonFireSpread)
# status: pass

def testonBlockExploded(e):
	logout(f'Listener onBlockExploded s_e["actor"].name = {e["actor"].name}, l_e["blockpos"] = {list_to_str(e["blockpos"])}, s_e["blockname"] = {e["blockname"]}, i_e["blockid"] = {e["blockid"]}, i_e["dimensionid"] = {e["dimensionid"]}')
	#mc.removeListener("onBlockExploded", testonBlockExploded)
#mc.setListener("onBlockExploded", testonBlockExploded)
# status: pass

def testonUseSignBlock(e):
	logout(f'Listener onUseSignBlock i_e["player"].platform = {e["player"].platform}, s_e["text"] = {e["text"]}, l_e["position"] = {e["position"]}')
	#mc.removeListener("onUseSignBlock", testonUseSignBlock)
#mc.setListener("onUseSignBlock", testonUseSignBlock)


def testonPreJoin(e):
    logout(f'Listener onPreJoin e.ip = {e.ip}, e.name = {e.name}')
# status: pass

#temple
def test(e):
	logout(f'Listener ')
	
# #mc.setListener('onUseItem', testonUseItem)
# #mc.setListener('onPlaceBlock', testonPlaceBlock)
# #mc.setListener('onDestroyBlock', testonDestroyBlock)
# #mc.setListener('onOpenContainer', testonOpenContainer) # onOpenChest, onOpenBarrel -> onOpenContainer
# #mc.setListener('onCloseContainer', testonCloseContainer) # onCloseChest, onCloseBarrel -> onCloseContainer
# #no output
# #mc.setListener('onContainerChange', testonContainerChange)
# #no full output
# #mc.setListener('onChangeDim', testonChangeDim) # onChangeDimension -> onChangeDim
# #mc.setListener('onMobDie', testonMobDie)
# #mc.setListener("onMobHurt", testonMobHurt)
# #mc.setListener("onRespawn", testonRespawn)
# #mc.setListener("onChat", testonChat)
# #mc.setListener("onInputText", testonInputText)
# #mc.setListener("onCommandBlockUpdate", testonCommandBlockUpdate)
# #mc.setListener("onPlayerCmd", testonPlayerCmd) # onInputCommand -> onPlayerCmd
# #mc.setListener("onCmdBlockExecute", testonCmdBlockExecute) # onCommandBlockPerform -> onCmdBlockExecute
# #mc.setListener("onLevelExplode", testonLevelExplode)
# #mc.setListener("onSetArmor", testonSetArmor)
# #mc.setListener("onFarmLandDecay", testonFarmLandDecay) # onFallBlockTransform -> onFarmLandDecay
# #mc.setListener("onUseRespawnAnchor", testonUseRespawnAnchor) # onUseRespawnAnchorBlock -> onUseRespawnAnchor
# #mc.setListener("onScoreChanged", testonScoreChanged)
# #no output
# #-#mc.setListener("onMove", testonMove) # frist test this listener and then disable it to test other listener
# #mc.setListener("onPistonPush", testonPistonPush)
# #-#mc.setListener("onEndermanRandomTeleport", testonEndermanRandomTeleport)
# #-#mc.setListener("onServerStarted", testonServerStarted)
# #-#mc.setListener("onDropItem", testonDropItem)
# #-#mc.setListener("onTakeItem", testonTakeItem)
# #mc.setListener("onRide", testonRide)
# #mc.setListener("onUseFrameBlock", testonUseFrameBlock)
# #-#mc.setListener("onJump", testonJump)
# #-#mc.setListener("onSneak", testonSneak)
# #mc.setListener("onBlockInteracted", testonBlockInteracted)
# #mc.setListener("onFireSpread", testonFireSpread)
# #mc.setListener("onBlockExploded", testonBlockExploded)
# #mc.setListener("onUseSignBlock", testonUseSignBlock)
# #no ouput
# # temple
# #mc.setListener("", test)


'''
def testonOpenChest(e):
	printLog('Listener onOpenChest l_e['position'] = （%f,%f,%f）, e['player'].typeid = %d' % (e['position'][0], e['position'][1], e['position'][2], e['player'].typeid))
	#mc.removeListener

def testonCloseChest(e):
	printLog('Listener onCloseChest l_e['position'] = %f, e['player'].typename = %s' % (e['position'][0], e['position'][1], e['position'][2], e['player'].typename))
	#mc.removeListener

def testonOpenBarrel(e):
	printLog('Listener onOpenBarrel l_e['position'] = （%f,%f,%f）, e['player'].platform_online_id = %s' % (e['position'][0], e['position'][1], e['position'][2], e['player'].platform_online_id))
	#mc.removeListener

def testonCloseBarrel(e):
	printLog('Listener onCloseBarrel l_e['position'] = （%f,%f,%f）, e['player'].platform = %d' % (e['position'][0], e['position'][1], e['position'][2], e['player'].platform))
	#mc.removeListener
'''
##mc.setListener('onJoin', testonPlayerJoin)
##mc.setListener('onLeft', testonPlayerLeft)
##mc.setListener('onConsoleInput', testonConsoleInput)
##mc.setListener('onOpenBarrel', testonOpenBarrel)
##mc.setListener('onOpenChest', testonOpenChest)
##mc.setListener('onCloseChest', testonCloseChest)
##mc.setListener('onCloseBarrel', testonCloseBarrel)
##mc.setListener('onChangeDimension', testonChangeDimension)
#mc.setListener("onPreJoin", testonPreJoin)
