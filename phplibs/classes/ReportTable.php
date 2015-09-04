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

    public static function closeTable($tableName) {
        echo "</tbody></table>
            <script>
                $(function(){
                    //$('\#$tableName').tablesorter();
                    $('\#$tableName').DataTable(
                    {
                    'paging': false,
                    'info': false,
                    'order': [], //no initial order
                    'language': {
                        'search': 'Find _INPUT_ in search results'
                      }
                    } );
                });
            </script>";
    }

    public static function closeTable2 ($tableName) {
        ?>
        </tbody></table>
        <script>


            function filterColumn ( i, useRegex, useSmartSearch ) {
                $('<?php echo $tableName; ?>').DataTable().column( i ).search(
                    $('#col'+i+'_filter').val(), useRegex, useSmartSearch
                ).draw();
            }

            $(document).ready(function() {
                $('<?php echo $tableName; ?>').dataTable(
                {
                'paging': false,
                'info': false,
                'order': [], //no initial order
                'language': {
                    'search': 'Find _INPUT_ in filtered results'
                  } ,
                "search": {
                    "regex": true,
                    "smart": false
                  }
                } );

                $('input.column_filter').on( 'keyup click', function () {
                    filterColumn( $(this).parents('tr').attr('data-column'), true, false );
                } );
            } );
        </script>
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
                "ajax": "/async/cnstdwglog.php?jobinfo_id=<?php echo $jobinfo_id; ?>&section=<?php echo $section;?>&last_issuance_id=<?php echo $issuance_id; ?>",
                "deferRender": true
                } );

                $('input.column_filter').on( 'keyup click', function () {
                    filterColumn( $(this).parents('tr').attr('data-column'), true, false );
                } );
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

    public static function boss_search_filter ($initial_index, $search_terms, $tablename) {
        ?>
        <div class="tablesearcherholder">
            <div style="float:left;">Filter data</div>
            <script>
                function clear_inputs ()
                {
                    window.location.reload(true);
                }
            </script>
            <div class="reset_control">
                <a class="std_link" href="javascript:clear_inputs()">
                    reset
                </a>
            </div>
            <br/>
            <table class='tablesearcher' cellpadding="3" cellspacing="0" border="0">
                <tbody>
                <?php
                for ($i = $initial_index, $j = 0; $j < count($search_terms); $i++, $j++) {
                    ?>
                    <tr id="filter_col<?php echo $j + 1; ?>" data-column="<?php echo $i; ?>">
                        <td><?php echo $search_terms[$j]; ?></td>
                        <td align="center"><input placeholder='search text' type="text" class="column_filter"
                                                  id="col<?php echo $i; ?>_filter"></td>
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