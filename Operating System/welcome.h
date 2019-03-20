#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <random>

using namespace std;

std::vector<std::string> system_logo();

void display_logo()
{
	std::vector<std::string> data = system_logo();
	system("pause");
	system("cls");
	srand(time(NULL));
	int color = rand() % 11;
	switch (color)
	{
	case 0:
	{
		std::cout << blue;
		break;
	}
	case 1:
	{
		std::cout << cyan;
		break;
	}
	case 2:
	{
		std::cout << magenta;
		break;
	}
	case 3:
	{
		std::cout << gray;
		break;
	}
	case 4:
	{
		std::cout << red;
		break;
	}
	case 5:
	{
		std::cout << green;
		break;
	}
	case 6:
	{
		std::cout << yellow;
		break;
	}
	case 7:
	{
		std::cout << darkgreen;
		break;
	}
	case 8:
	{
		std::cout << darkblue;
		break;
	}
	case 9:
	{
		std::cout << darkred;
		break;
	}
	case 10:
	{
		std::cout << white;
		break;
	}
	}

	for (int i = 0; i < 148; i = i + 10)
	{
		system("cls");
		for (int j = 0; j < 29; j++)
		{
			if (i - j < 0)
			{
				cout << endl;
			}
			else if (i - j > 120)
			{
				for (int k = 0; k < 120; k++)
				{
					cout << data[j][k];
				}
			}
			else
			{
				for (int k = 0; k < i - j; k++)
				{
					cout << data[j][k];
				}

			}
			cout << endl;
		}
		for (int time = 0; time < 20000000; time++) {}
	}
	std::cout << white;
	system("pause");
	system("cls");
}

std::vector<std::string> system_logo()
{
	std::string temp;
	std::vector<std::string> welcome;
	temp = "       `....-:---::...`                                   `.......`                              ..::::--`              ";
	welcome.push_back(temp);
	temp = "  -ysoyoohooyyyssy++ooydyoo/.                       `-+++sysssd/syys++:.                    `::yh+oyydhhshys/-`         ";
	welcome.push_back(temp);
	temp = "  :mysyyyysssssdhy+ymhymyooyyh+.                 `/:sh+ydys/++-++ho+s/sdyo-               ./++yhohdhhhoohyo++/ss-       ";
	welcome.push_back(temp);
	temp = "  :ymdsyhoo+shsosomNddMs/ooyysyms`             .::/s++NNNho/:+ossooosyysdyss:`           oooss+smyomyshhsy+oydmyhy/`    ";
	welcome.push_back(temp);
	temp = "  :dyyss++sds+oyydhshhoosdNo+yhdyo.          .+:/+o+smmhs//+syso/++sysss+ossdh-         +yys++s+/yyohy+ys/ohyyyyssso+`  ";
	welcome.push_back(temp);
	temp = "  :ysss/ohs+.`````.-:+osdy+ohmho+/o         +so+++yyydy++syhysy+osoooooos+sdssho`      /hys/osssso:::-:/ososyssossomm+  ";
	welcome.push_back(temp);
	temp = "  -so+sdssoy`          `:y++so/+os+:      `+so++syo++/oy+/-.````../+sy/+oddysoss+`     ososyooo+o:       `:oos+++ys/.   ";
	welcome.push_back(temp);
	temp = "  -oods+s+sy`            `so/oyossy+      shyhyyo++/+y/`            `:oshNhyyhs++o`    yyysyy+/yh/          .++ss-      ";
	welcome.push_back(temp);
	temp = "  -hs+oosyyo`            `sshsyyssy/     /ydhyoyy+/s/                 `+mssysysydy:    sosyo/shsoo+.          -`        ";
	welcome.push_back(temp);
	temp = "  :soosyyoss`           /dmyyhsyhsy-    -yds:syh++s-                    odsooshdsoy.   `ss/shyyhshmy+.                  ";
	welcome.push_back(temp);
	temp = "  -oodd+oyoo`    `..:/omhhyhshhhyo:     sh//sso+sy-                     `ss+yh++y+h+    .+ssodhyhs+yh+o:                ";
	welcome.push_back(temp);
	temp = "  :dm++sss+hhsoms:yyhmhhyyshdyyo+/     :y+oo++yhs+        BASIC          :hy+:yyhd+y`     /sssydosdyhyoyo/`             ";
	welcome.push_back(temp);
	temp = "  :y++ohsodNssyohhoosoosyhmyysso.      :++//shsyy-                       `y+yysddh/o-      `+yy+osdhhdNhdhy+.           ";
	welcome.push_back(temp);
	temp = "  -y+yysdNhyhsddys+o+++sdsysoyh:       :s/hd+ossy.        OPERATING       hh+yhmo:ss-         -oddhmdhyyohdhhs.         ";
	welcome.push_back(temp);
	temp = "  -dhsdNmhhsyyyss+s++oyo++syssdho+`    -oy++soyyo`                        oyymy/sdyo-           `+yoyyohmdmhooyo.       ";
	welcome.push_back(temp);
	temp = "  -hhdyssyoo/+:/o:/+ohsoso++sdyoy++.   :o/syyho/y-        SYSTEM         `hys:yNsohh-              :+sdhdysshysoy:      ";
	welcome.push_back(temp);
	temp = "  -ss+oysosy`         `:ooohNho++syh.  .syhmyoyy+/                       /y+odhyddss`                `+osysmssdyos+`    ";
	welcome.push_back(temp);
	temp = "  -yossoshso`            -ymyo/oNNyo+   odhoshys+h-       v1.48.8       .ooddhmdhdyo                   `:sysydmyo+:+    ";
	welcome.push_back(temp);
	temp = "  -ysooyy++s`             oos/ymdyso+   :y+osyoomyy.                    ydhosyysmmy`        `            .+ymhys+:yd+   ";
	welcome.push_back(temp);
	temp = "  -hssooosos`            `yoomdoysyy+    +hhyymNNmyy-                 -sdhsssshNhs/       `:so-           .mdo/:+s+sy`  ";
	welcome.push_back(temp);
	temp = "  -yosdNhsoo`          `:::ymsssyhdh.    `symdhdsshyyo-             .+hmhys/oNhso/      `:+sydhs/`         y//sy+++oo-  ";
	welcome.push_back(temp);
	temp = "  -hhNmyshys-.....-+:++--sdsoshhddy/      `ooysymhyd+syo+-.`    `.:odydmh//mdo+ss     `/ysdMdmdyyho/:.` `.:ody+oossos-  ";
	welcome.push_back(temp);
	temp = "  -s+oohh//+oys+:+ohy-`/yo+yhdhdhh:         +yyhdhsomhs+syoosoossshhydd+:sdo/oh:       /hNmymd/ooyhosydysomh/sdhysooy`  ";
	welcome.push_back(temp);
	temp = "  -ysydyooohmh++ssy:.+y+/sysyhos+`           .yyoysdhoodhoosssyhddsss/-+yo:omh-         .oyds+symy+odmhoso/sshmhsoyo.   ";
	welcome.push_back(temp);
	temp = "  -shdysoodddsooy/-oho/oooyy+-.                ./oyosyhhyssshmMNysh::oy+:+do-             `/oohh++sssyhyoss+hyohhm+.    ";
	welcome.push_back(temp);
	temp = "  `yyyyosyyss/oo/shyosh+::-`                     `:/shyhsoshdds+hs/hh+-oo+.                  `/+oo/oyyysmhho+ohho-      ";
	welcome.push_back(temp);
	temp = "  `yyyyosyyss/oo/shyosh+::-`                     `:/shyhsoshdds+hs/hh+-oo+.                  `/+oo/oyyysmhho+ohho-      ";
	welcome.push_back(temp);
	temp = "  `....---:--:--..``                                .:++syss/oo+yys/--.                         `.:+:+os/::/:.          ";
	welcome.push_back(temp);
	temp = "                                                            ....``                                                      ";
	welcome.push_back(temp);

	return welcome;
}

std::vector<std::string> system_creators()
{
	std::vector<std::string> data;
	std::string temp;
	temp = "Tworcy systemu BOS v1.28,8                                                                                      ";
	data.push_back(temp);
	temp = "     Stanislaw Kucznerowicz - Zarzadzanie procesorem metoda karuzelowa                                          ";
	data.push_back(temp);
	temp = "          Kacper Chrul  - Mechanizmy synchronizacyjne, zmienne warunkowe, zamki                                 ";
	data.push_back(temp);
	temp = "               Tomasz Adamczyk - Zarzadzanie pamiecia RAM metoda stronnicowania                                 ";
	data.push_back(temp);
	temp = "                    Paulina Drozda - Pamiec wirtualna metoda LRU                                                ";
	data.push_back(temp);
	temp = "                         Mikolaj Debski -  Zarzadzanie procesami wedlug ideologii Linuxa                        ";
	data.push_back(temp);
	temp = "                              Arek Dudziak - Komunikacja miedzyprocesowa za pomoca potokow nienazwanych z linuxa";
	data.push_back(temp);
	temp = "                                   Krystian Duchnowski - System plikow i katalogow metoda mieszana              ";
	data.push_back(temp);
	temp = "                                        Jacek Eichler - Rozkazy asemblerowe i interpreter                       ";
	data.push_back(temp);
	temp = "                                             Dawid Solomon - Interfejs, Shell                                   ";
	data.push_back(temp);
	return data;
}

void display_creators()
{
	std::vector<std::string> data = system_creators();
	system("pause");
	system("cls");

	for (int i = 0; i < 121; i = i + 10)
	{
		system("cls");
		for (int j = 0; j < 10; j++)
		{

			if (i - j < 0)
			{
				cout << endl;
			}
			else if (i - j > 112)
			{
				for (int k = 0; k < 112; k++)
				{
					switch (j)
					{
					case 0:
					{
						cout << white << data[j][k] << black;
						break;
					}
					case 1:
					{
						cout << cyan << data[j][k] << black;
						break;
					}
					case 2:
					{
						cout << red << data[j][k] << black;
						break;
					}
					case 3:
					{
						cout << darkred << data[j][k] << black;
						break;
					}
					case 4:
					{
						cout << gray << data[j][k] << black;
						break;
					}
					case 5:
					{
						cout << darkblue << data[j][k] << black;
						break;
					}
					case 6:
					{
						cout << darkgreen << data[j][k] << black;
						break;
					}
					case 7:
					{
						cout << blue << data[j][k] << black;
						break;
					}
					case 8:
					{
						cout << yellow << data[j][k] << black;
						break;
					}
					case 9:
					{
						cout << magenta << data[j][k] << black;
						break;
					}
					}
				}
			}
			else
			{
				for (int k = 0; k < i - j; k++)
				{
					switch (j)
					{
					case 0:
					{
						cout << white << data[j][k] << black;
						break;
					}
					case 1:
					{
						cout << cyan << data[j][k] << black;
						break;
					}
					case 2:
					{
						cout << red << data[j][k] << black;
						break;
					}
					case 3:
					{
						cout << darkred << data[j][k] << black;
						break;
					}
					case 4:
					{
						cout << gray << data[j][k] << black;
						break;
					}
					case 5:
					{
						cout << darkblue << data[j][k] << black;
						break;
					}
					case 6:
					{
						cout << darkgreen << data[j][k] << black;
						break;
					}
					case 7:
					{
						cout << blue << data[j][k] << black;
						break;
					}
					case 8:
					{
						cout << yellow << data[j][k] << black;
						break;
					}
					case 9:
					{
						cout << magenta << data[j][k] << black;
						break;
					}
					}
				}
			}
			cout << endl << endl;
		}
		for (int time = 0; time < 20000000; time++) {}
	}
	system("pause");
	system("cls");
}