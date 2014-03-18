<?php

class ScrNavTestSetGet {
    
    static function set(&$scr) {
        
        $scr->setInterval(21);			// Сколько объектов на странице
        $scr->setPrefix('?qwe&part=');		// URL prefix
        $scr->setPostfix('&prm=132');		// URL prefix
        //$scr->setSpace('<space>...</space>');	// Разделитель блоков табов
        //$scr->setCssName('newClassName');	// Имя класса css блока управления постраничным выводом
        $scr->setMidTab(15);			// см. info.png
        $scr->setMaxTab(5);			// см. info.png
        $scr->showCount(true);			// Показывать ли общее количество элементов
        
    }
    
    static function set1(&$scr) {
        
        $scr->setInterval(21);			// Сколько объектов на странице
        $scr->setPrefix('?qwe&part=');		// URL prefix
        $scr->setPostfix('&prm=132');		// URL prefix
        $scr->setSpace('<space>...</space>');	// Разделитель блоков табов
        $scr->setCssName('newClassName');	// Имя класса css блока управления постраничным выводом
        $scr->setMidTab(15);			// см. info.png
        $scr->setMaxTab(5);			// см. info.png
        $scr->showCount(false);			// Показывать ли общее количество элементов
        
    }
    
    static function get(&$scr) {
        
        self::get1($scr);
        echo $scr->show();
        
    }
    
    static function get1(&$scr) {
        
        var_export(array(
            'StartPos' => $scr->getStartPos(),
            'LimitPos' => $scr->getLimitPos(),
            'PageCnt'  => $scr->getPageCnt(),
            'PageNo'   => $scr->getPageNo(),
            'start'    => $start = $scr->getStartPos(),
            'lim'      => $start+$scr->getLimitPos()
        ));
        
    }
    
    
}