LoadResource("Resources.xml")
GUI:LoadLayers("Layers.xml")

-- Загрузка эффектов
LoadEffects("Explosion.xml")
LoadEffects("Jet.xml")
LoadEffects("Texts.xml")

-- Ресурсы для отладки и информирования
UploadResourceGroup("InfoGroup")
UploadResourceGroup("Intro")
UploadResourceGroup("Game")

-- Отображаем первым слой уровня на данный момент
Screen:pushLayer("IntroLayer")
