SilentGame
【制作者】　グループ（自身含め４人）
【制作日】　３年前期
【種　別】　ゲーム
【動作環境】Windows10
【開発環境】
　　VisualStudio2022
　　DxLib
　　Tiled
―――――――――――――――――――――――――――――――――――――

・実行に必要なプログラムとデータファイル
　　"Project"ファイル内の.exeから実行してください。
・開発に使用したソースファイル
　　"Project"ファイル内にあります。
・実行画面をキャプチャした動画ファイル
　　"Video"ファイル内にあります。


・操作方法
　　十字キー　　　　　移動
　　LSHIFT　　　　　　ダッシュ
　　LCtrl　　　　　　 スロー移動
　　Z　　　　　　　　 アイテム使用
　　X,C　　　　　　　 アイテム欄カーソル移動

・制作ヵ所
　　・全般担当
	AnimMngクラス
	ImageMngクラス
	Controllerクラス
	InputID.h
	Keyboardクラス
	Padクラス
	Effectクラス
	EffectID.h
	ItemUiクラス
	Staminaクラス
	UiBaseクラス
	UiMngクラス
	PlayerState.h
	CircleSceneクラス
	ClearTransクラス
	DeadTransクラス
　　・一部担当
	Animationクラス（Update、コメント記入以外を担当）
	Playerクラス（ノーマルマップ処理以外を担当）
	Objクラス（GetPos()やIsAttack()などのプレイヤー、エフェクトに関する処理を担当）
	ObjManagerクラス（PlayerとEffectに関する処理を担当）
	BaseSceneクラス（コメント記入以外担当）
	GameSceneクラス（PlayerとEffect、シーン移行処理等を担当）