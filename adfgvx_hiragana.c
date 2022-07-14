// 'A'はA\0を組み合わせたもの. 'A'はA. c言語は'と'を区別する.
// textfは空白を取得できない
// 戻り値がvoidの関数はreturn 0しない
//ファイルに出力
//復号するときはキーワードを求めるようにする

#include <stdio.h>
#include <wchar.h>
#include <locale.h> 

int itsEndIndexNumIs(wchar_t string[]); //指定したワイド文字列配列の中で一番後ろにある文字のインデックス番号を返す.一文字も入ってない場合-1を返す.
int itsIndexNumIs(wchar_t word); //指定した平仮名がひらがな表の[0][0]の要素を「1」、[8][8]の要素を「81」として、その規則に従った値を返す。見つからなかったら-1を返す。
void copyArray(wchar_t assigned[], wchar_t exist[]);

wchar_t table[9][9] ={
            {L'あ',L'い',L'う',L'え',L'お',L'か',L'き',L'く',L'け'},
            {L'こ',L'さ',L'し',L'す',L'せ',L'そ',L'た',L'ち',L'つ'},
            {L'て',L'と',L'な',L'に',L'ぬ',L'ね',L'の',L'は',L'ひ'},
            {L'ふ',L'へ',L'ほ',L'ま',L'み',L'む',L'め',L'も',L'や'},
            {L'ゆ',L'よ',L'ら',L'り',L'る',L'れ',L'ろ',L'わ',L'を'},
            {L'ん',L'が',L'ぎ',L'ぐ',L'げ',L'ご',L'ざ',L'じ',L'ず'},
            {L'ぜ',L'ぞ',L'だ',L'ぢ',L'づ',L'で',L'ど',L'ば',L'び'},
            {L'ぶ',L'べ',L'ぼ',L'ぱ',L'ぴ',L'ぷ',L'ぺ',L'ぽ',L'っ'},
            {L'ゃ',L'ゅ',L'ょ',L'！',L'？',L'「',L'」',L'（',L'）'}
        };


int main(int argc, const char * argv[]){
    setlocale(LC_CTYPE, ""); //ワイド文字列を出力するために必要
    //--------換字をするための準備----------
    wchar_t transformKey[9] = {L'き',L'さ',L'ま',L'み',L'て',L'い',L'る',L'な',L'!'};
    wchar_t arrangeKey[11] = {L'\0'}; //パスワードは10文字以内
    wchar_t text[501] = {L'\0'}; //暗号化する文章を入れるやつ
    wchar_t codedText[1000][20] = { {L'\0'} }; //暗号化した文章を入れるやつ
    int findFlag = 0;
    int indexToInput = 0; //scanfを無限ループして,改行処理をするために使う
    //------------------------------------

    printf("-----------------パスワードを設定してください-----------------\n");
    printf("＊10文字以内\n");
    printf("＊ひらがなのみ\n");
    printf("---------------------------------------------------------\n");
    printf("パスワード : ");
    scanf("%ls",arrangeKey);

    //------入力されたパスワードをあいうえお順にソートする------
    wchar_t sort_arrange[11] = {L'\0'};
    copyArray(sort_arrange, arrangeKey); //sort_arrangeへarrangeKeyの要素をコピー
    wchar_t tmp = '\0';
    int preNum = 0; //ひらがな表の位置を入れる
    int postNum = 0; //ひらがな表の位置を入れる
    for(int i=0; i < itsEndIndexNumIs(sort_arrange) + 2; i++){ //バブルソート
        for(int j=0; j < itsEndIndexNumIs(sort_arrange) ; j++){ //　隣同士の比較を行う
            preNum = itsIndexNumIs(sort_arrange[j]);
            postNum = itsIndexNumIs(sort_arrange[j+1]); 
            if(preNum > postNum){
                tmp = sort_arrange[j];
                sort_arrange[j] = sort_arrange[j+1];
                sort_arrange[j+1] = tmp;
            }
        }
    }
    //--------------------------------------------------

    printf("-----------------文を入力してください。(qで終了)-----------------\n");
    printf("使用可能文字: ひらがな\n");
    printf("使用可能文字: ！, ？, 」,「, ）,（　 ←すべて全角\n");
    printf("注:改行や空白を空けるなどは未実装です。\n");
    printf("----------------------↓入力してください(qで終了)------------------------\n");

    while(1){ //scanfを無限ループ
        indexToInput = itsEndIndexNumIs(text); //textfする前の最後尾にある文字のインデックス番号を代入
        scanf("%ls",&text[indexToInput + 1]);
        indexToInput = itsEndIndexNumIs(text); //textf後の最後尾にある文字のインデックス番号を代入
        if(text[indexToInput] == 'q'){ //qが入力されたらループ終了
            break;
        }
    }

    int count = 0; //設定したパスワードの1文字目から、あいうえお順にしたときに何番目にあるかを調べるときに使う
    int rowIncrement = 0;

    for(int i = 0; i < sizeof(text); i++){ //textの文字をひとつずつチェック

        if(text[i] == 'q'){ //textから取り出した値が'\0'だったら全ての文字を見終えたのでtext内の探索を中断する
            printf("\n");
            break;
        }

        for(int r = 0; r < 9; r++){  //textから取り出した文字が表の何行何列にあるかを確かめる r=行 c=列
            for(int c = 0; c < 9; c++){
                if(text[i] == table[r][c]){ //textから取り出した文字が見つかったら
                        for(int j = 0; sort_arrange[j] != '\0'; j++){ //arrangeとsortが一致するまで
                            if(arrangeKey[count] == sort_arrange[j]){ //設定したパスワードの、指定した位置にある文字と、それをあいうえお順にしたときの位置が等しいiになったら
                                if(codedText[rowIncrement][2*j] == '\0'){ //パスワードに同じ平仮名があったら上書きしてしまうので,重複の場合は2個後ろの要素に入れる
                                    codedText[rowIncrement][2*j] = transformKey[r];
                                    codedText[rowIncrement][2*j+1] = transformKey[c];
                                } else if(codedText[rowIncrement][2*j+2] == '\0'){
                                    codedText[rowIncrement][2*j+2] = transformKey[r];
                                    codedText[rowIncrement][2*j+3] = transformKey[c];
                                } else{ //重複が3文字あったとき用
                                    codedText[rowIncrement][2*j+4] = transformKey[r];
                                    codedText[rowIncrement][2*j+5] = transformKey[c];
                                }
                                break;
                            }
                        }

                        findFlag = 1; //表から文字が見つかったらそれを出力してflagを立てる
                        count++;//パスワードの次の文字へ
                        if(arrangeKey[count] == '\0'){
                            count = 0;
                            rowIncrement++;
                        }
                        break; //列のインクリメントを中断
                }
            }
            if(findFlag == 1){ //値が見つかったら,行のインクリメントを中断
                findFlag = 0;
                break;
            }
        }
    }

    rowIncrement++;
    codedText[rowIncrement][0] = 'q';

    for(int r = 0; codedText[r][0] != 'q'; r++){
        for(int c = 0; arrangeKey[c] != '\0'; c++){
            wprintf(L"%lc%lc", codedText[r][2*c], codedText[r][2*c+1]);
        }
    }

    printf("\n");    
    return 0;
}


int itsEndIndexNumIs(wchar_t string[]){
    int count;
    for(count = 0; string[count] != '\0'; count++){

    }
    return count - 1; //配列の最後尾にある文字のインデックス番号を返す.
}

int itsIndexNumIs(wchar_t word){
    int count = 0;

     for(int r = 0; r < 9; r++){  //ひらがな表探索
            for(int c = 0; c < 9; c++){ //ひらがな表探索
                count++;
                if(word == table[r][c]){
                    return count;
                }
            }
        }
    
    return -1;
}

void copyArray(wchar_t assigned[], wchar_t exist[]){
    for(int i = 0; exist[i] != '\0'; i++){
        assigned[i] = exist[i];
    }
}