rename table `areatriggers` to `areatrigger_extra`;
alter table `areatrigger_extra` drop column `map`, drop column `position_x`, drop column `position_y`, drop column `position_z`, drop column `orientation`;