<h1 align="center">💬 多言語対応チャットサーバー – ネットワーク & ローカライズデモ</h1>
<p align="center">
  本プロジェクトは、<strong>C++によるネットワークプログラミング</strong>の<strong>実践的な経験</strong>を通じて、  
  <strong>マルチスレッド型サーバー設計</strong>および<strong>多言語テキスト処理</strong>への理解を深めることを目的として制作しました。<br>
  リアルタイムチャット環境を題材とし、同時接続制御、パケットの信頼性、UTF-8ベースのローカライズ課題に取り組んでいます。
</p>

<p align="center">
  <a href="./README.md">English</a> |
  <a href="./README_jp.md">日本語</a>
</p>


## 🌟 ハイライト
<table>
  <tr>
    <td style="text-align:center">
      <img src="https://github.com/user-attachments/assets/680fa7b4-2c07-45b0-80b2-10107a378926" width="400" height="300">
    </td>
    <td style="text-align:center">
      <img src="https://github.com/user-attachments/assets/e903cfc3-2c40-4f90-adee-240cd365009c" width="400" height="300">
    </td>
  </tr>
</table>
<br>

## 🔗 リンク
- [デモ動画](https://youtu.be/K_OWQfoCe0c)

<br>

## 📜 プロジェクト概要
- **開発期間**：`2025.09.07 ~ 2025.09.14`
- **開発人数**：`1名（個人開発）`
- **主なテーマ**：`マルチスレッド`, `ネットワーク`, `パケット設計`, `ローカライズ`

<br>

## ⚙️ 開発環境
- **使用言語**：`C++17`
- **ネットワークAPI**：`WinSock2`
- **開発ツール**：`Visual Studio 2024`

<br>

## ⚒️ 実装内容
- **サーバー構成**：スレッドプールを用いたマルチスレッドサーバー設計と共有リソースの同期制御。
- **クライアント構成**：送信・受信処理を分離したスレッド構造により、入力待ちによるブロッキングを回避。
- **パケット処理**：ヘッダベースのパケット規約を定義し、明示的な直列化・逆直列化を実装。
- **言語テキスト処理**：自動改行処理と、クライアント側での簡易ワードフィルタリング機能。

<br>

## ⚠️ 注意事項
- 本プロジェクトは **学習および技術検証を目的とした個人開発** です。
- 現在は **Blocking I/O** を前提とした実装となっており、  
  今後は **IOCPなどの非同期通信方式や性能最適化** を検討しています。
