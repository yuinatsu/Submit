#pragma once

//--------------------------------------------------------------------------
// FilePass：画像ファイルのパスを格納
//--------------------------------------------------------------------------
// [名前付けルール（暫定）]
// ・全て大文字、アンダーバーで表記する
// ・画像ファイルはGH（Graphic Handleの略）、音声ファイルはSH（Sound Handleの略）を先頭に付ける
// （Visual StudioのIntelliCodeの検索結果に引っかかりやすくするため）
// ・文字はLOGO、背景はBG（backgroundの略）、武器はICONのようにカテゴリ分けする
// [備考]
// ・特に理由がない限り画像ファイルはPNG形式、音声ファイルはMP3形式にしておきます
//--------------------------------------------------------------------------

//【画像ファイル】
#define GH_ICON_BLOCK ("image/block.png")//ブロック
#define GH_ICON_ITEM ("image/color.png")//アイテム
#define GH_ICON_CURSOR ("image/cursor.png")//カーソル

#define GH_LOGO_TITLE ("image/titleLogo.png")//タイトルシーンタイトルロゴ
#define GH_LOGO_SELECT_KEY ("image/key.gif")//タイトルシーンキーロゴ
#define GH_LOGO_TITLE_FRAME ("image/frame.jpeg")//タイトルシーンキーロゴ用フレーム
#define GH_BG_TITLE ("image/titleScreen.jpg")//タイトルシーン背景

#define GH_LOGO_SELECT ("image/selectLogo.png")//セレクトシーンセレクトロゴ

#define GH_LOGO_GAME_CALL ("image/call.png")//ゲームシーンカウントダウンロゴ
#define GH_ICON_GAME_RULE ("image/rule.png")//ゲームシーンルール説明

#define GH_LOGO_MENU ("image/menu.png")//メニューロゴ

#define GH_LOGO_CLEAR ("image/clearLogo.png")//クリアシーンクリアロゴ
#define GH_BG_CLEAR ("image/clearScreen.jpeg")//クリアシーン背景

//【音声ファイル】
#define SH_BGM_TITLE ("music/titleBGM.mp3")//タイトルシーンBGM
#define SH_SE_TITLE ("music/titleSE.mp3")//タイトルシーンSE

#define SH_BGM_SELECT ("music/selectBGM.mp3")//セレクトシーンBGM
#define SH_SE_SELECT ("music/selectSE.mp3")//セレクトシーンSE

#define SH_BGM_GAME1 ("music/gameBGM1.mp3")//ゲームシーンBGM1
#define SH_BGM_GAME2 ("music/gameBGM2.mp3")//ゲームシーンBGM2
#define SH_BGM_GAME3 ("music/gameBGM3.mp3")//ゲームシーンBGM3
#define SH_SE_GAME_ITEM ("music/itemBGM.mp3")//ゲームシーンアイテムSE
#define SH_SE_GAME_CLEAR ("music/clearSE.mp3")//ゲームシーンクリアSE

#define SH_SE_CLEAR ("music/backSE.mp3")//クリアシーンSE

//【システムファイル】


