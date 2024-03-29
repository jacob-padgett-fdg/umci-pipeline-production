<?php

namespace classes\ReportTable;

class RowSpec {
    var $displayName;
    var $columnName;
    var $tableName;
}

class ReportTable {
    var $baseQuery;
    var $rowSpec;
    var $formName;
    var $outputElementName;

    public function ReportTable($query, $rs, $formName, $outputElementName) {
        $this->baseQuery = $query;
        $this->rowSpec = $rs;
        $this->formName = $formName;
        $this->outputElementName = $outputElementName;
    }

    public static function closeTable2 ($tableName, $appname) {
        ?>
        </tbody></table>
        <script>

            function filterColumn ( i, useRegex, useSmartSearch ) {
                myValue = $('#col'+i+'_filter').val();
                $('<?php echo $tableName; ?>').DataTable().column( i ).search(
                    myValue, useRegex, useSmartSearch
                ).draw();
            }

            $(document).ready(function() {
                $('<?php echo $tableName; ?>').dataTable(
                {
                'bStateSave' : true,
                'paging': true,
                'info': false,
                'order': [], //no initial order
                'language': {
                    'search': 'Find _INPUT_ in filtered results'
                  } ,
                "search": {
                    "regex": true,
                    "smart": false
                  },
                "stateSave": true
                });
                $('input.column_filter').on( 'keyup click', function () {
                    filterColumn( $(this).parents('tr').attr('data-column'), true, false );
                } );
                $('select.column_filter').on( 'change', function () {
                        var val = $.fn.dataTable.util.escapeRegex(
                            $(this).val()
                        );

                        $('<?php echo $tableName; ?>').DataTable().column( $(this).parents('tr').attr('data-column') )
                            .search( val ? '^'+val+'$' : '', true, false )
                            .draw();
                    } );
            });
        </script>
        <?php
    }


    public static function cnstdwglogSearchFilters($initial_index, $search_terms, $issuance_list) {
        //initially - from boss
        ?>
        <div class="tablesearcherholder">
            <? ReportTable::doCommonControls('#cnstdwglog'); ?>

            <table class='tablesearcher' cellpadding="3" cellspacing="0" border="0">
                <tbody>
                <?php
                $i = $initial_index;
                $j = 0;
                while ($j < count($search_terms)) {
                    ?>
                    <tr id="filter_col<?php echo $j + 1; ?>" data-column="<?php echo $i; ?>">
                        <td><?php echo $search_terms[$j]; ?></td>
                        <td align="center"><input placeholder='search text' type="text" class="column_filter"
                                                  id="col<?php echo $i; ?>_filter"></td>
                    </tr>
                <?php
                    $i++;
                    $j++;
                }
                if (($_SESSION['cnstdwglog_view'] == "expanded") && ($issuance_list)) {
                    reset($issuance_list);
                    $current_id = current($issuance_list);
                    $current_name = next($issuance_list);
                    $current_type = next($issuance_list);
                    $current_current = next($issuance_list);
                    while (!($current_id === FALSE)) {
                        ?>
                        <tr id="filter_col<?php echo $j + 1; ?>" data-column="<?php echo $i; ?>">
                            <td><?php echo $current_name; ?></td>
                            <td align="center"><input placeholder='enter x to filter' type="text" class="column_filter"
                                                      id="col<?php echo $i; ?>_filter"></td>
                        </tr>

                        <?php

                        $current_id = next($issuance_list);
                        $current_name = next($issuance_list);
                        $current_type = next($issuance_list);
                        $current_current = next($issuance_list);
                        $i++;
                        $j++;
                    }
                }
                ?>
                </tbody>
            </table>
        </div>
        <?php
    }

    public static function cnstdwglogTable ($jobinfo_id, $section, $issuance_id) {
        ?>
        </tbody></table>
        <script>


            function filterColumn ( i, useRegex, useSmartSearch ) {
                $('#cnstdwglog').DataTable().column( i ).search(
                    $('#col'+i+'_filter').val(), useRegex, useSmartSearch
                ).draw();
            }



            $(document).ready(function() {
                $('#cnstdwglog').dataTable(
                {
                'paging': true,
                'info': false,
                'order': [], //no initial order
                'language': {
                    'search': 'Find _INPUT_ in filtered results'
                  } ,
                "search": {
                    "regex": false,
                    "smart": true
                  },

                /*"processing": true,
                "serverSide": true,*/
                "ajax": "/async/cnstdwglog/cnstdwglog.php?jobinfo_id=<?php echo $jobinfo_id; ?>&section=<?php echo $section;?>&last_issuance_id=<?php echo $issuance_id; ?>",
                "deferRender": true,
                "autoWidth": false,
                "stateSave": true,
                "bStateSave": true
                } );
            } );

            $('input.column_filter').on( 'keyup click', function () {
                filterColumn( $(this).parents('tr').attr('data-column'), true, false );
            } );
        </script>
        <?php
    }


    public static function generateTableHeader($rowHeaders, $tableId, $pagename, $color4, $color3)
    {

        echo "<table class='standard' id='$tableId' border=1 cellpadding='0' cellspacing='3'>
            <thead>
            <tr bgcolor='$color4'>";

        // the first column is special
        echo
        "<th class='" . $rowHeaders[0]['class']. "'>
            <a href=$pagename?mode=main&csvdump=1><span style=\"background: $color3; border: 3px solid black;\" color=blue>" . $rowHeaders[0]['name']. "</span></a>
        </th>";
        for ($i = 1; $i < count($rowHeaders); $i++) {
            echo "<th class=\"" . $rowHeaders[$i]['class'] . "\"><span>" . $rowHeaders[$i]['name'] . "</span></th>";
        }
        echo "</tr></thead><tbody>";
    }

    public static function doCommonControls($tableName) {
        ?>
            <div style="float:left;">Filter data</div>
            <script>
                $(document).on( 'init.dt', function ( e, settings ) {
                    var api = new $.fn.dataTable.Api( settings );
                    var state = api.state.loaded();
                    // ... use `state` to restore information for filters
                    // must be done manually for some browsers e.g, Mozilla
                    if (null != state) {
                        for (i = 0; i < state.columns.length; i++) {
                            tmp = state.columns[i].search.search;
                            tmp = tmp.replace(/^\^/, '').replace(/\$$/,'').replace(/\\-/g, '-'); //strip out escape and terminal regex ^ and $
                            $('#col'+i+'_filter').val(tmp);
                        }
                    }
                } );

                function clear_inputs()
                {
                    $('input.column_filter').each(function( index ) {
                        $( this ).val('');
                    });
                    $('select.column_filter').each(function( index ) {
                        $( this ).val('');
                    });
                    $('.dataTable').DataTable()
                        .search( '' )
                        .columns().search( '' )
                        .draw();
                }
            </script>
            <div class="reset_control">
                <a class="std_link" href="javascript:clear_inputs()">
                    reset
                </a>
            </div>
            <br/>
        <?
    }
    public static function drawinglog_search_filter ($initial_index, $search_terms, $jobinfo_id) {
        ?>
        <div class="tablesearcherholder">
            <? ReportTable::doCommonControls('drawinglog'); //?>

            <table class='tablesearcher' cellpadding="3" cellspacing="0" border="0">
                <tbody>
                <?php
                for ($i = $initial_index, $j = 0; $j < count($search_terms); $i++, $j++) {
                    ?>
                    <tr id="filter_col<?php echo $j + 1; ?>" data-column="<?php echo $i; ?>">
                        <td><?php echo $search_terms[$j]; ?></td>
                        <td align="center">
                        <?php
                        if ('Type' == $search_terms[$j]) {
                            $query = "select distinct type from drawinglog where jobinfo_id='$jobinfo_id' order by type asc";
                            $res = @mysql_query($query);
                            echo "<select class='column_filter' id='col{$i}_filter'>";
                            echo "<option></option>";
                            while($row=@mysql_fetch_object($res)) {
                                echo "<option>{$row->type}</option>";
                            }
                            echo "</select>";
                        ?>
                            <!--input placeholder='<?php echo $query; ?>' type="text" class="column_filter" id="col<?php echo $i; ?>_filter"-->
                        <?php
                        } else {
                        ?>
                            <input placeholder='search text' type="text" class="column_filter"
                                                      id="col<?php echo $i; ?>_filter">
                        <?php
                        }
                        ?>
                        </td>
                    </tr>
                <?php
                }
                ?>
                </tbody>
            </table>
        </div>
    <?php
    }

}
